
/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:02:57 2012
 * @brief Object definitions for SFS.
 *
 * Object definitions for SFS.
 */

#include "object.h"
#include "mem.h"

extern int VERB_SWITCH;

/* fonction donnée dans le sujet qui crée un object et utilise l'argument pour initialiser le champ type. */
object make_object( uint type )
{

    object t = sfs_malloc( sizeof( *t ) );

    t->type = type;

    return t;
}

/* fonction donnée dans le sujet qui crée une liste vide */
object make_nil( void )
{

    object t = make_object( SFS_NIL );

    t->this.special = t;

    return t;
}

/* A partir de la ce sont nos fonctions */

/* Création d'entier, a changer pour nombre dans le futur */
object make_integer ( int Valeur_integer )
{

    object t = make_object( SFS_INTEGER );

    t->this.integer = Valeur_integer;

    return t;
}


/* Création de symbole */
object make_symbol ( char* Valeur_symbol )
{

    object t = make_object( SFS_SYMBOL );

    strcpy( t->this.symbol, Valeur_symbol );

    return t;
}

/* Création d'une chaine de caractères */
object make_string ( char* Valeur_string )
{

    object t = make_object( SFS_STRING );

    strcpy( t->this.string, Valeur_string );

    return t;
}

/* Création d'un caractère */
object make_character ( char Valeur_character )
{

    object t = make_object( SFS_CHARACTER );

    t->this.character = Valeur_character;

    return t;
}

/* Création d'une paire */
object make_pair( object Valeur_car, object Valeur_cdr)
{

    object t = make_object ( SFS_PAIR );

    t->this.pair.car = Valeur_car;
    t->this.pair.cdr = Valeur_cdr;

    return t;
}


/* création des booleens true et false */
object make_boolean( unsigned int Valeur_boolean)
{

    object t = make_object ( SFS_BOOLEAN );

    if ( Valeur_boolean == 0)
    {

        strcpy( t->this.symbol, "#f" );
    }


    else
    {

        strcpy( t->this.symbol, "#t" );
    }

    return t;
}

/* création des primitives */
object make_primitive( object (*Valeur_fonction)(object))
{

    object t = make_object ( SFS_PRIMITIVE );

    t->this.primitive.fonction=Valeur_fonction;

    if (0 && VERB_SWITCH)
        printf("make_primitive : NEW %p\n",t );

    return t;
}

object make_compound( object Valeur_param, object Valeur_body, object Valeur_env )
{
    object t = make_object ( SFS_COMPOUND );

    t->this.compound.param=Valeur_param;
    t->this.compound.body=Valeur_body;
    t->this.compound.env=Valeur_env;

    if (0 && VERB_SWITCH)
        printf("make_compound : NEW %p\n",t );

    return t;
}

object Car(object Obj)
{
    return(Obj->this.pair.car);
}


object Cdr(object Obj)
{
    return(Obj->this.pair.cdr);
}


