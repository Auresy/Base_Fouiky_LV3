#include "environement.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "mem.h"
#include "primitive.h"

extern object ENV_TETE;
extern int VERB_SWITCH;

/* Fonctions d'environnement pour SCHEME */


object ENV_maillon() /*Alloue un maillon pour l'environement*/
{
    return (make_pair(NULL, NULL));
}


void ENV_init() /*Initialise les variables d'environement SCHEME et la globale object* "ENV_TETE"*/
{
    ENV_TETE = ENV_maillon();

    if (VERB_SWITCH)
    {
        printf("ENV_MEM INIT!\n");
        printf("%p\n", ENV_TETE );
    }


    /* création d'une librairie de primitives standard*/

    creation_librairie();

}

void ENV_NewEnv(void* papa) /*Prépare un nouvel environnement qui pointe vers son papa*/
{
    object Ne = ENV_maillon();
    Ne->this.pair.car = papa;

    if (VERB_SWITCH)
    {
        printf("ENV_NewEnv: Nouvel arrivant !\n");
        printf("%p\n", Ne );
    }

}


void ENV_definir(char* Symbole, object Obj) /*Associe une chaine "Symbole" à un objet*/
{
    object Pt = ENV_TETE;

    if ( ENV_est_defini(Symbole) )
    {
        ENV_redefinir(Symbole, Obj);
    }

    else
    {
        /*se placer en queue du level*/
        while(Car(Pt) != NULL)
        {
            Pt = Car(Pt);
        }

        if (VERB_SWITCH)
            printf("Definir %s %p\n", Symbole, Pt );

        /*Creer paire suite*/
        object St = ENV_maillon();
        /*Creer paire hôte*/
        object Ht = ENV_maillon();
        /*Association du nom symbole*/
        Ht->this.pair.cdr = make_symbol(Symbole);
        Ht->this.pair.car = Obj;
        /*Lien suite/hôte*/
        St->this.pair.car = NULL;
        St->this.pair.cdr = Ht;
        /*Lien suite/suite*/
        Pt->this.pair.car = St;
    }
}


void ENV_redefinir(char* Symbole, object Obj) /*recherche puis change le car de l'hote mémoire*/
{
    object Pt = ENV_TETE;
    int i = 1;
    while(Car(Pt) != NULL && i)
    {
        Pt = Car(Pt);

        if( !strcmp( Cdr(Cdr(Pt))->this.symbol, Symbole) )
        {
            if (VERB_SWITCH)
                printf("ENV_redefinir : %s trouvé\n", Symbole);

            Pt->this.pair.cdr->this.pair.car = Obj;
            i = 0;
        }
    }

    if(VERB_SWITCH && i)
        printf(" ENV_redefinir : %s pas trouvé, non redefini\n", Symbole);
}


int ENV_est_defini(char* Symbole) /*Renvoie 1 si la chaîne Symbole est deja affectée 0 sinon*/
{
    object Pt = ENV_TETE;

    while(Car(Pt) != NULL)
    {
        Pt = Car(Pt);

        if( !strcmp( Cdr(Cdr(Pt))->this.symbol, Symbole) )
        {
            if (VERB_SWITCH)
                printf("ENV_est_defini : %s est défini\n", Symbole);

            return (1);
        }
    }

    if (VERB_SWITCH)
        printf(" %s pas défini\n", Symbole);

    return(0);
}

object ENV_chercher( char* Symbole ) /*Renvoie l'objet associé à la chaîne Symbole s'il existe et NULL sinon*/
{
    object Pt = ENV_TETE;

    while(Car(Pt) != NULL)
    {
        Pt = Car(Pt);

        if( !strcmp( Cdr(Cdr(Pt))->this.symbol, Symbole) )
        {
            if (VERB_SWITCH)
                printf("ENV_chercher : %s trouvé\n", Symbole);

            if ( Car(Cdr(Pt))->type == SFS_SYMBOL && ENV_chercher( Car(Cdr(Pt))->this.symbol ) != NULL )
            {
                return ( ENV_chercher( Car(Cdr(Pt))->this.symbol ) );
            }

            return (Car(Cdr(Pt)));
        }
    }

    if(VERB_SWITCH)
        printf("ENV_chercher : %s pas trouvé\n", Symbole);

    /*printf("ERREUR ENV_chercher, recherche infructueuse\n");
    exit( EXIT_FAILURE );*/
    return(NULL);

}


void ENV_supprimer(char* Symbole) /*Supprime et libère la memoire pour le symbole donné en liant la chaine*/
{
    if( !ENV_est_defini(Symbole) )
    {
        if(VERB_SWITCH)
            printf("ENV_supprimer : Aucun symbole de ce nom\n");

        exit(EXIT_FAILURE);
    }

    /*Recuperation des pointeurs cdr, car, St, Ht*/
    object Pt = ENV_TETE;
    object Ps = ENV_TETE;

    void* P1 = NULL;
    void* P2 = NULL;
    void* P3 = NULL;
    void* P4 = NULL;


    int Cleared = 0;

    while(Car(Pt) != NULL && !Cleared)
    {
        if ( !strcmp(Cdr(Cdr(Car(Pt)))->this.symbol, Symbole) )
        {
            Ps->this.pair.car = Car(Car(Pt));

            P1 = Cdr(Cdr(Car(Pt)));
            P2 = Car(Cdr(Car(Pt)));
            P3 = Car(Car(Pt));
            P4 = Cdr(Car(Pt));

            Cleared = 1;
        }

        Pt = Car(Pt);
    }

    Pt->this.pair.car = Car(Ps);

    /*Liberation de la memoire pour Branche&Feuilles*/
    if (VERB_SWITCH)
        printf("ENV_supprimer : %s supprimé\n", Symbole );

    sfs_free(P1);
    sfs_free(P2);
    sfs_free(P3);
    sfs_free(P4);
}

void ENV_purifier() /*libère l'environement en supprimant toutes les variables puis en detruisant la tête*/
{
    printf("ENV_purifier : Purifier HS\n");
    
    if (0)
    {

    /*suppression tant que non vide*/
    object Pt = ENV_TETE;

    while(Car(Pt) != NULL)
    {
        ENV_supprimer( Cdr(Cdr(Car(Pt)))->this.symbol );
        /*printf("%s Supprimé\n");*/
    }

    free(Pt);

    if (VERB_SWITCH)
        printf("PURIFIED\n");

    } /* HS */

}

void ENV_List() /*Liste les variables déclarées dans l'environnement*/
{
    object Pt = ENV_TETE;

    while(Car(Pt) != NULL)
    {
        printf("%s\n",Cdr(Cdr(Car(Pt)))->this.symbol );
        Pt = Car(Pt);
    }

}