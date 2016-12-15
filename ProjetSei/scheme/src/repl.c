
/**
 * @file repl.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:13:02 2012
 * @brief Main REPL for SFS.
 *
 * Main REPL for SFS.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "decoupe.h"

/*#include "ctype.h"*/
#include "object.h"
#include "environement.h"
#include "read.h"
#include "eval.h"
#include "print.h"

/* mode d'interaction avec l'interpreteur (exemple)*/
typedef enum {INTERACTIF,SCRIPT} inter_mode;


void usage_error( char *command )
{

    fprintf( stderr, "Usage: %s [file.scm]\n   If no file is given, executes in Shell mode.\n", command );
}


object nil;
object T;
object F;

object ENV_TETE;

/* controle de la verbosité 1 = SPAM ou 0 = Je fais des test d'erreur tranquillou*/
int VERB_SWITCH =1;


void init_interpreter ( void )
{

    nil = make_nil();
    T=make_boolean(1);
    F=make_boolean(0);
}

int main ( int argc, char *argv[] )
{

    char     input[BIGSTRING];
    uint     here = 0;
    object   output = NULL;
    object   sexpr = NULL;
    inter_mode mode;
    FILE *   fp = NULL; /* le flux dans lequel les commande seront lues : stdin (mode shell) ou un fichier */

    /* exemples d'utilisation des macros du fichier notify.h */
    /* WARNING_MSG : sera toujours affiche */
    /*WARNING_MSG("Un message WARNING_MSG !");*/

    /* macro INFO_MSG : uniquement si compil avec -DVERBOSE. Cf Makefile*/
    /*INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]);*/

    /* macro DEBUG_MSG : uniquement si compil avec -DDEBUG (ie : compil avec make debug). Cf Makefile */
    /*DEBUG_MSG("Un message DEBUG_MSG !");*/

    /* La ligne suivante provoquerait l'affichage du message
       puis la sortie du programme avec un code erreur non nul (EXIT_FAILURE) */
    /* ERROR_MSG("Erreur. Arret du programme"); */

    if ( argc > 2 )
    {
        usage_error( argv[0] );
        exit( EXIT_FAILURE );
    }

    if(argc == 2 && strcmp(argv[1], "-h") == 0)
    {
        usage_error( argv[0] );
        exit( EXIT_SUCCESS );
    }

    init_interpreter();


    /* OBJETS DES TESTS
        object integer=make_integer(46);
        object symb=make_symbol("cons");
        object str=make_string("bonjour");
        object c=make_character('a');
        object pair=make_pair(integer,nil);
        object pair2=make_pair(integer,pair);*/

    /* TEST FONCTIONS MAKES ///////////////////////////////////////////////////////////////////////

        printf("int %d \nsymb %s \nstr %s\nchar %c \ncar %s\ncdr %d\n type %d\n %d\n %d\n", integer->this.integer, symb->this.symbol, str->this.string, c->this.character, pair->this.pair.car->this.symbol, pair->this.pair.cdr->this.integer, integer->type, symb->type, str->type);

        printf ("true : %s \n false : %s \n", T->this.symbol, F->this.symbol);
    //////////////////////////////////////////////////////////////////////////////////                   */

    /* TEST FONCTIONS PRINT //////////////////////////////////////////////////////////////////////
        sfs_print_atom(integer);
        sfs_print_atom(symb);
        sfs_print_atom(str);
        sfs_print_atom(c);
        printf("\n\n\n");
        sfs_print(pair);
        sfs_print(pair2);
    /////////////////////////////////////////////////////////////*/


    /*par defaut : mode shell interactif */
    fp = stdin;
    mode = INTERACTIF;

    if(argc == 2)
    {
        /* mode fichier de commandes */
        fp = fopen( argv[1], "r" );

        if ( fp == NULL )
        {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }

        mode = SCRIPT;
    }

    /*Definition environnement & tests*/

    /*printf("%p\n", ENV_TETE );*/

    ENV_init();
    /*printf("%p\n", ENV_TETE );
    printf("%p\n", Car(ENV_TETE) );
    printf("%p\n", Cdr(ENV_TETE) );*/

    /*ENV_definir("X", make_integer(12) );
    printf("%p\n", ENV_TETE );
    printf("%p\n", Car(ENV_TETE) );
    printf("%p\n", Cdr(ENV_TETE) );

    ENV_est_defini("X");

    object sch = ENV_chercher("X");
    sfs_print(sch);
    printf("\n");

    ENV_supprimer("X");
    ENV_est_defini("X");
    ENV_definir("Xa", make_integer(12) );

    ENV_purifier();

    exit(EXIT_SUCCESS);*/


    while ( 1 )
    {
        input[0]='\0';
        here = 0;
        output = NULL;
        sexpr = NULL;
        uint Sexpr_err;

        Sexpr_err = sfs_get_sexpr( input, fp );

        if ( S_OK != Sexpr_err)
        {
            /* si fichier alors on sort*/
            if (mode == SCRIPT)
            {
                fclose( fp );

                if (Sexpr_err==S_END)
                {
                    /* Cas fin de fichier script */
                    exit(EXIT_SUCCESS);
                }

                /* Cas S-Expression mal formee dans le fichier script */
                ERROR_MSG("Malformed S-expression --- Aborts");
            }

            /*sinon on rend la main à l'utilisateur*/
            continue;
        }

        if ( 0 == strlen( input ) )
        {
            continue;
        }

        /* Pour le step 0 on se contente d'afficher */
        /* la S-Expression lue */
        /* puis d'en lire une autre */
        /* METTRE EN COMMENTAIRE LES DEUX LIGNES SUIVANTES */
        /* POUR PASSER A L'INCREMENT 1
        printf("%s\n", input );
        continue; */

        here  = 0;

        if (strcmp(input, "quit") == 0)
        {
            if (VERB_SWITCH)
                printf("Fin session utilisateur \n");

            ENV_purifier();
            exit(EXIT_SUCCESS);
        }

        if (strcmp(input, "SPAM") == 0)
        {
            VERB_SWITCH = !VERB_SWITCH;

            if (VERB_SWITCH)
                printf("SPAM ON !\n");

            else
                printf("SPAM OFF !\n");

            continue ;
        }

        if (strcmp(input, "LISTVAR") == 0)
        {
            ENV_List( ENV_TETE );
            continue;
        }

        if (strcmp(input, "DELVAR") == 0)
        {
            ENV_supprimer( Cdr(Cdr(Car(ENV_TETE)))->this.symbol, ENV_TETE );
            continue;
        }

        sexpr = sfs_read( input, &here );

        if ( NULL == sexpr )
        {
            /* si fichier alors on sort*/
            if (mode == SCRIPT)
            {
                fclose( fp );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("Invalid S-expression --- Aborts");
            }

            /*sinon on rend la main à l'utilisateur*/
            continue ;
        }

        output = sfs_eval( sexpr, ENV_TETE );

        if( NULL == output)
        {
            /* si fichier alors on sort*/
            if (mode == SCRIPT)
            {
                fclose( fp );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("Error while evaluating input --- Aborts");
            }

            /*sinon on rend la main à l'utilisateur*/
            continue ;
        }

        printf( "==> " );
        sfs_print( output );
        printf( "\n" );
    }

    if (mode == SCRIPT)
    {
        fclose( fp );
    }

    exit( EXIT_SUCCESS );
}
