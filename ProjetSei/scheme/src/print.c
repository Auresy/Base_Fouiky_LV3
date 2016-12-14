
/**
 * @file print.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:17 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"

#include <stdio.h>
/* Cette fonction affiche les atomes qu'elle prend en argument */
void sfs_print_atom( object o )
{
    switch(o->type)
    {
        case SFS_INTEGER :
            printf("%d",o->this.integer);
            break;

        case SFS_CHARACTER :
            printf("#\\");
            printf("%c",o->this.character);
            break;

        case SFS_STRING :
            printf("\"");
            printf("%s",o->this.string);
            printf("\"");
            break;

        case SFS_NIL :
            printf("()");
            break;

        case SFS_BOOLEAN :
            printf("%s",o->this.symbol);
            break;

        case SFS_SYMBOL :
            printf("%s",o->this.symbol);
            break;

        case SFS_COMPOUND :
            printf("<#procédure>");
            break;

        case SFS_PRIMITIVE :
            printf("<#fonction>");
            break;

        default :
            printf("ERREUR sfs_print_atom, l'élément à afficher n'est pas un ATOME !\n");
            exit( EXIT_FAILURE );
            break;
    }
}

/* Cette fonction affiche les paires en les séparant et appelant la fonction print récursivement sur le car puis le cdr*/
void sfs_print_pair( object o )
{

    sfs_print(o->this.pair.car);

    if ( o->this.pair.cdr->type == SFS_PAIR)
    {
        printf(" ");
        sfs_print_pair(o->this.pair.cdr);
    }
    else if (o->this.pair.cdr->type != SFS_NIL)
    {
        printf(".");
        sfs_print_atom(o->this.pair.cdr);
    }

}

/*cette fonction est donnée et utilise les autres fonction print pour afficher récursivement. */
void sfs_print( object o )
{

    if ( SFS_PAIR == o->type )
    {
        printf("(");
        sfs_print_pair( o );
        printf(")");
    }

    else
    {
        sfs_print_atom( o );
    }

}
