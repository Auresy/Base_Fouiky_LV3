#include "environement.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "mem.h"
#include "primitive.h"
#include "eval.h"

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

object ENV_NewEnv(object papa) /*Prépare un nouvel environnement qui pointe vers son papa*/
{
    object Ne = ENV_maillon();
    Ne->this.pair.cdr = papa;

    if (VERB_SWITCH)
    {
        printf("ENV_NewEnv: Nouvel arrivant !\n");
        printf("%p\n", Ne );
    }
    return (Ne);

}


void ENV_definir(char* Symbole, object Obj, object EnvC ) /*Associe une chaine "Symbole" à un objet*/
{
    if ( ENV_est_defini(Symbole, EnvC) )
    {
        ENV_redefinir(Symbole, Obj, EnvC);
    }

    else
    {
        /*se placer en queue du level*/
        while(Car(EnvC) != NULL)
        {
            EnvC = Car(EnvC);
        }

        if (VERB_SWITCH)
            printf("Definir %s %p\n", Symbole, EnvC );

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
        EnvC->this.pair.car = St;
    }
}


void ENV_redefinir(char* Symbole, object Obj, object EnvC) /*recherche puis change le car de l'hote mémoire*/
{
    object Tete = EnvC;
    int i = 1;
    while(Tete != NULL)
    {
        while(EnvC != NULL && Car(EnvC) != NULL && i)
        {
            EnvC = Car(EnvC);

            if( !strcmp( Cdr(Cdr(EnvC))->this.symbol, Symbole) )
            {
                if (VERB_SWITCH)
                    printf("ENV_redefinir : %s trouvé\n", Symbole);

                EnvC->this.pair.cdr->this.pair.car = Obj;
                i = 0;
            }
        }
        Tete = Cdr(Tete);
        EnvC = Tete;
    }

    if(VERB_SWITCH && i)
        printf(" ENV_redefinir : %s pas trouvé, non redefini\n", Symbole);
}


int ENV_est_defini(char* Symbole, object EnvC) /*Renvoie 1 si la chaîne Symbole est deja affectée 0 sinon*/
{
    object Tete = EnvC;
    while( Tete != NULL)
    {
        while(Car(EnvC) != NULL)
        {
            EnvC = Car(EnvC);

            if( !strcmp( Cdr(Cdr(EnvC))->this.symbol, Symbole) )
            {
                if (VERB_SWITCH)
                    printf("ENV_est_defini : %s est défini\n", Symbole);

                return (1);
            }
        }
        Tete = Cdr(Tete);
        EnvC = Tete;
    }
    if (VERB_SWITCH)
        printf(" %s pas défini\n", Symbole);

    return(0);
}

object ENV_chercher( char* Symbole, object EnvC ) /*Renvoie l'objet associé à la chaîne Symbole s'il existe et NULL sinon*/
{
    object Tete = EnvC;
    while( Tete != NULL )
    {

        while(Car(EnvC) != NULL)
        {
            EnvC = Car(EnvC);

            if( !strcmp( Cdr(Cdr(EnvC))->this.symbol, Symbole) )
            {
                if (VERB_SWITCH)
                    printf("ENV_chercher : %s trouvé\n", Symbole);

                if ( Car(Cdr(EnvC))->type == SFS_SYMBOL && ENV_chercher( Car(Cdr(EnvC))->this.symbol ) != NULL )
                {
                    return ( ENV_chercher( Car(Cdr(EnvC))->this.symbol ) );
                }

                return (Car(Cdr(EnvC)));
            }
        }
        Tete = Cdr(Tete);
        EnvC = Tete;
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

void ENV_List( EnvC ) /*Liste les variables déclarées dans l'environnement*/
{
    object Tete = EnvC;
    while( Tete != NULL )
    {
        while(Car(EnvC) != NULL)
        {
            printf("%s\n",Cdr(Cdr(Car(EnvC)))->this.symbol );
            EnvC = Car(EnvC);
        }
        Tete = Cdr( Tete );
        EnvC = Tete;
        printf(">> ENV + 1 <<\n");
    }
}