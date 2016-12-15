#include "primitive.h"
#include "eval.h"
#include "object.h"
#include "environement.h"

#include <stdlib.h>

extern object ENV_TETE;

/* Creation des objets primitives et intégration au Top level. */
void creation_librairie(void)
{
    object EnvC = ENV_TETE;

    object t;
    t=make_primitive( add );
    ENV_definir("+", t, EnvC);

    t=make_primitive( sub );
    ENV_definir("-", t, EnvC);

    t=make_primitive( multiplie );
    ENV_definir("*", t, EnvC);

    t=make_primitive( quotient );
    ENV_definir("quotient", t, EnvC);

    t=make_primitive( remainder );
    ENV_definir("remainder", t, EnvC);

    t=make_primitive( equal );
    ENV_definir("=", t, EnvC);

    t=make_primitive( sup );
    ENV_definir("<", t, EnvC);

    t=make_primitive( inf );
    ENV_definir(">", t, EnvC);

    t=make_primitive( predicatnull );
    ENV_definir("null?", t, EnvC);

    t=make_primitive( predicatboolean );
    ENV_definir("boolean?", t, EnvC);

    t=make_primitive( predicatsymbol );
    ENV_definir("symbol?", t, EnvC);

    t=make_primitive( predicatinteger );
    ENV_definir("integer?", t, EnvC);

    t=make_primitive( predicatchar );
    ENV_definir("char?", t, EnvC);

    t=make_primitive( predicatstring );
    ENV_definir("string?", t, EnvC);

    t=make_primitive( predicatpair );
    ENV_definir("pair?", t, EnvC);

    t=make_primitive( predicatprocedure );
    ENV_definir("procedure?", t, EnvC);

    t=make_primitive( predicatequal );
    ENV_definir("eq?", t, EnvC);

    t=make_primitive( cons );
    ENV_definir("cons", t, EnvC);

    t=make_primitive( primitivecar );
    ENV_definir("car", t, EnvC);

    t=make_primitive( primitivecdr );
    ENV_definir("cdr", t, EnvC);

    t=make_primitive( setcar );
    ENV_definir("set-car!", t, EnvC);

    t=make_primitive( setcdr );
    ENV_definir("set-cdr!", t, EnvC);

    t=make_primitive( list );
    ENV_definir("list", t, EnvC);

    /*FouinyBaby*/

    t=make_primitive( qtype );
    ENV_definir("type", t, EnvC);

    t=make_primitive( charToint );
    ENV_definir("char->integer", t, EnvC);

    t=make_primitive( intTochar );
    ENV_definir("integer->char", t, EnvC);

    t=make_primitive( numTostring );
    ENV_definir("number->string", t, EnvC);

    t=make_primitive( stringTonum );
    ENV_definir("string->number", t, EnvC);
/*
        t=make_primitive( symbolTostring );
        ENV_definir("symbol->string", t);

        t=make_primitive( stringTosymbol );
        ENV_definir("string->symbol", t);
    */
}

/* Opérations arithmétiques*/
object add(object input)
{

    /* verifications*/
    object verif=input;
    if (verif->type == SFS_INTEGER) return verif;

    do
    {
        if ( verif->type != SFS_PAIR || Car(verif)->type != SFS_INTEGER )
        {
            printf("Entier attendu \n");
            return NULL;
        }

        verif=Cdr(verif);
    }
    while (verif->type != SFS_NIL);

    /*calcul*/
    int sum=0;
    sum=Car(input)->this.integer;

    while ( Cdr(input)->type != SFS_NIL )
    {
        sum+=Car(Cdr(input))->this.integer;
        input=Cdr(input);
    }

    return make_integer(sum);
}

object sub(object input)
{

    /* verifications*/
    object verif=input;

    if (verif->type == SFS_INTEGER) return verif;

    do
    {
        if (verif->type != SFS_PAIR || Car(verif)->type != SFS_INTEGER )
        {
            printf("Entier attendu \n");
            return NULL;
        }

        verif=Cdr(verif);
    }
    while (verif->type != SFS_NIL);

    int sum=0;
    sum=Car(input)->this.integer;

    while ( Cdr(input)->type != SFS_NIL )
    {
        sum-=Car(Cdr(input))->this.integer;
        input=Cdr(input);
    }

    return make_integer(sum);
}

object multiplie(object input)
{

    /* verifications*/
    object verif=input;

    if (verif->type == SFS_INTEGER) return verif;

    do
    {
        if (verif->type != SFS_PAIR || Car(verif)->type != SFS_INTEGER )
        {
            printf("Entier attendu \n");
            return NULL;
        }

        verif=Cdr(verif);
    }
    while (verif->type != SFS_NIL);

    int sum=0;
    sum=Car(input)->this.integer;

    while ( Cdr(input)->type != SFS_NIL )
    {
        sum*=Car(Cdr(input))->this.integer;
        input=Cdr(input);
    }

    return make_integer(sum);
}

object quotient(object input)
{

    /* verifications*/
    object verif=input;

    if (verif->type == SFS_INTEGER) return verif;

    do
    {
        if (verif->type != SFS_PAIR || Car(verif)->type != SFS_INTEGER )
        {
            printf("Entier attendu \n");
            return NULL;
        }

        verif=Cdr(verif);
    }
    while (verif->type != SFS_NIL);

    if (Car(input)->type != SFS_INTEGER || Car(Cdr(input))->type != SFS_INTEGER || Cdr(Cdr(input))->type != SFS_NIL)
    {
        printf(" Le quotient prends deux arguments \n");
        return NULL;
    }

    return make_integer(Car(input)->this.integer/Car(Cdr(input))->this.integer);
}

object remainder(object input)
{

    /* verifications*/
    object verif=input;

    if (verif->type == SFS_INTEGER) return verif;

    do
    {
        if (verif->type != SFS_PAIR || Car(verif)->type != SFS_INTEGER )
        {
            printf("Entier attendu \n");
            return NULL;
        }

        verif=Cdr(verif);
    }
    while (verif->type != SFS_NIL);

    if (Car(input)->type != SFS_INTEGER && Car(Cdr(input))->type != SFS_INTEGER && Cdr(Cdr(input))->type != SFS_NIL)
    {
        printf(" Le remainder prends deux arguments \n");
        return NULL;
    }

    return make_integer(Car(input)->this.integer%Car(Cdr(input))->this.integer);
}

object equal(object input)
{

    /* verifications*/
    object verif=input;

    if (verif->type == SFS_INTEGER) return verif;

    do
    {
        if (verif->type != SFS_PAIR || Car(verif)->type != SFS_INTEGER )
        {
            printf("Entier attendu \n");
            return NULL;
        }

        verif=Cdr(verif);
    }
    while (verif->type != SFS_NIL);

    int sum=0;
    sum=Car(input)->this.integer;

    while ( Cdr(input)->type != SFS_NIL )
    {
        if ( sum != Car(Cdr(input))->this.integer)
        {
            return F;
        }

        input=Cdr(input);
    }

    return T;
}

object sup(object input)  /* SUP = <  */
{

    /* verifications*/
    object verif=input;

    if (verif->type == SFS_INTEGER) return verif;

    do
    {
        if (verif->type != SFS_PAIR || Car(verif)->type != SFS_INTEGER )
        {
            printf("Entier attendu \n");
            return NULL;
        }

        verif=Cdr(verif);
    }
    while (verif->type != SFS_NIL);

    int sum=0;
    sum=Car(input)->this.integer;

    while ( Cdr(input)->type != SFS_NIL )
    {
        if ( sum >= Car(Cdr(input))->this.integer)
        {
            return F;
        }

        input=Cdr(input);
    }

    return T;
}

object inf(object input)  /* INF = >  */
{

    /* verifications*/
    object verif=input;

    if (verif->type == SFS_INTEGER) return verif;

    do
    {
        if (verif->type != SFS_PAIR || Car(verif)->type != SFS_INTEGER )
        {
            printf("Entier attendu \n");
            return NULL;
        }

        verif=Cdr(verif);
    }
    while (verif->type != SFS_NIL);

    int sum=0;
    sum=Car(input)->this.integer;

    while ( Cdr(input)->type != SFS_NIL )
    {
        if ( sum <= Car(Cdr(input))->this.integer)
        {
            return F;
        }

        input=Cdr(input);
    }

    return T;
}

/*Prédicats */

object predicatnull(object input)
{
    if (input->type == SFS_NIL) return T;

    else
        if (input->type != SFS_PAIR) return F;

    if (Car(input)->type != SFS_NIL)
    {
        return F;
    }

    while ( Cdr(input)->type != SFS_NIL )
    {
        input=Cdr(input);

        if (Car(input)->type != SFS_NIL)
        {
            return F;
        }

    }

    return T;
}

object predicatboolean(object input)
{
    if (input->type == SFS_BOOLEAN) return T;

    else
        if (input->type != SFS_PAIR) return F;

    if (Car(input)->type != SFS_BOOLEAN)
    {
        return F;
    }

    while ( Cdr(input)->type != SFS_NIL )
    {
        input=Cdr(input);

        if (Car(input)->type != SFS_BOOLEAN)
        {
            return F;
        }

    }

    return T;
}

object predicatsymbol(object input)
{
    if (input->type == SFS_SYMBOL) return T;

    else
        if (input->type != SFS_PAIR) return F;

    if (Car(input)->type != SFS_SYMBOL)
    {
        return F;
    }

    while ( Cdr(input)->type != SFS_NIL )
    {
        input=Cdr(input);

        if (Car(input)->type != SFS_SYMBOL)
        {
            return F;
        }

    }

    return T;
}

object predicatinteger(object input)
{
    if (input->type == SFS_INTEGER) return T;

    else
        if (input->type != SFS_PAIR) return F;

    if (Car(input)->type != SFS_INTEGER)
    {
        return F;
    }

    while ( Cdr(input)->type != SFS_NIL )
    {
        input=Cdr(input);

        if (Car(input)->type != SFS_INTEGER)
        {
            return F;
        }

    }

    return T;
}

object predicatchar(object input)
{
    if (input->type == SFS_CHARACTER) return T;

    else
        if (input->type != SFS_PAIR) return F;

    if (Car(input)->type != SFS_CHARACTER)
    {
        return F;
    }

    while ( Cdr(input)->type != SFS_NIL )
    {
        input=Cdr(input);

        if (Car(input)->type != SFS_CHARACTER)
        {
            return F;
        }

    }

    return T;
}

object predicatstring(object input)
{
    if (input->type == SFS_STRING) return T;

    else
        if (input->type != SFS_PAIR) return F;

    if (Car(input)->type != SFS_STRING)
    {
        return F;
    }

    while ( Cdr(input)->type != SFS_NIL )
    {
        input=Cdr(input);

        if (Car(input)->type != SFS_STRING)
        {
            return F;
        }

    }

    return T;
}

object predicatpair(object input)
{
    if (input->type == SFS_PAIR) return T;

    else
        if (input->type != SFS_PAIR) return F;

    if (Car(input)->type != SFS_PAIR)
    {
        return F;
    }

    while ( Cdr(input)->type != SFS_NIL )
    {
        input=Cdr(input);

        if (Car(input)->type != SFS_PAIR)
        {
            return F;
        }

    }

    return T;
}

object predicatprocedure(object input)
{
    if (input->type == SFS_COMPOUND) return T;

    else
        if (input->type != SFS_PAIR) return F;

    if (Car(input)->type != SFS_COMPOUND)
    {
        return F;
    }

    while ( Cdr(input)->type != SFS_NIL )
    {
        input=Cdr(input);

        if (Car(input)->type != SFS_COMPOUND)
        {
            return F;
        }

    }

    return T;
}

object predicatequal(object input)
{
    if (input == NULL)
    {
        printf("Eq? : variable indéfinie détéctée\n");
        return(NULL);
    }
    object input1 = Car(input);
    object input2 = Car(Cdr(input));
    if (input1->type != input2->type)
    {
        printf("predicatequal : Types différents\n");
        return (NULL);
    }
    if (input1->type == SFS_INTEGER)
    {
        if ( input1->this.integer == input2->this.integer )
        {
            return(T);
        }
        else return(F);
    }
    if (input1->type == SFS_CHARACTER)
    {
        if ( input1->this.character == input2->this.character )
            {
                return(T);
            }
        else return(F);
    }
    if (input1->type == SFS_STRING)
    {
        if ( strcmp(input1->this.string, input2->this.string) )
            {
                return(F);
            }
        else return(T);
    }
    if (input1->type == SFS_PAIR)
    {
        if ( Car(input1) == Car(input2) && Cdr(input1) == Cdr(input2) )
            {
                return(T);
            }
        else return(F);
    }
    if (input1->type == SFS_NIL)
    {
        if (input2->type == SFS_NIL)
        {
            return(T);
        }
        else return(F);
    }
    if (input1->type == SFS_BOOLEAN)
    {
        if ( input2 == input1 )
        {
            return(T);
        }
        else return(F);
    }
    if (input1->type == SFS_SYMBOL)
    {
        if ( strcmp(input1->this.symbol, input2->this.symbol) )
            {
                return(F);
            }
        else return(T);
    }
    if (input1->type == SFS_PRIMITIVE)
    {
        if ( input1->this.primitive.fonction == input2->this.primitive.fonction )
            {
                return(T);
            }
        else return(F);
    }
    if (input1->type == SFS_COMPOUND)
    {
        printf("Ca Marche pas encore :) \n");
    }
    printf("EQ? pas traitable\n");
    return(NULL);
}

/* Manipulation de liste */

object cons(object input)
{
    if (Car(input)->type != SFS_INTEGER && Car(Cdr(input))->type != SFS_INTEGER && Cdr(Cdr(input))->type != SFS_NIL)
    {
        printf(" Le cons prends deux arguments \n");
        return NULL;
    }

    return make_pair( Car(input), Car(Cdr(input)));
}

object primitivecar(object input)
{
    if (Car(input)->type != SFS_PAIR)
    {
        printf(" Je ne peux pas donner le car d'un atome \n");
        return NULL;
    }

    return Car(Car(input));
}

object primitivecdr(object input)
{
    if (Car(input)->type != SFS_PAIR)
    {
        printf(" Je ne peux pas donner le cdr d'un atome \n");
        return NULL;
    }

    return Cdr(Car(input));
}

object setcar(object input)
{
    if (input->type != SFS_PAIR)
    {
        printf(" setcar n'a pas assez d'arguments \n");
        return NULL;
    }

    if (Car(input)->type != SFS_PAIR)
    {
        printf(" Je ne peux pas modifier le car d'un atome \n");
        return NULL;
    }

    if (Cdr(input)->type == SFS_NIL)
    {
        printf(" setcar n'a pas assez d'arguments \n");
        return NULL;
    }

    return make_pair( Car(Cdr(input)), Cdr(Car(input)) );
}

object setcdr(object input)
{
    if (input->type != SFS_PAIR)
    {
        printf(" setcar n'a pas assez d'arguments \n");
        return NULL;
    }

    if (Car(input)->type != SFS_PAIR)
    {
        printf(" Je ne peux pas modifier le car d'un atome \n");
        return NULL;
    }

    if (Cdr(input)->type == SFS_NIL)
    {
        printf(" setcar n'a pas assez d'arguments \n");
        return NULL;
    }

    return make_pair( Car(Car(input)), Car(Cdr(input)) );
}

object list(object input)
{
    return input;
}

/*Conversion de Types*/

object qtype (object input)
{
    printf("\nSFS_INTEGER      0x00\nSFS_CHARACTER    0x01\nSFS_STRING       0x02\nSFS_PAIR         0x03\nSFS_NIL          0x04\nSFS_BOOLEAN      0x05\nSFS_SYMBOL       0x06\nSFS_PRIMITIVE    0x07\nSFS_COMPOUND     0x08\n");
    printf("\n          %d\n\n", sfs_eval( Car(input), ENV_TETE )->type );
    return(input);
}

object charToint(object input)
{
    if ( Car(input)->type != SFS_CHARACTER )
    {
        printf("Char->integer : Operation impossible ceci n'est pas un SFS_CHARACTER\n");
        return NULL;
    }

    else
    {
        return( make_integer( atoi( &(Car(input)->this.character) ) ) );
    }
}

object intTochar(object input)
{
    if ( Car(input)->type != SFS_INTEGER )
    {
        printf("Char->integer : Operation impossible ceci n'est pas un SFS_INTEGER\n");
        return NULL;
    }

    else
    {
        return( make_character( (Car(input)->this.integer) ));
    }
}

object numTostring(object input)
{
    if ( Car(input)->type != SFS_INTEGER )
    {
        printf("Char->integer : Operation impossible ceci n'est pas un SFS_INTEGER\n");
        return NULL;
    }

    else
    {
        int i = Car(input)->this.integer ;
        char str[64];
        sprintf(str, "%d", i);
        return( make_string( str ) );
    }
}


object stringTonum(object input)
{
    if ( Car(input)->type != SFS_STRING )
    {
        printf("string->number : Operation impossible ceci n'est pas un SFS_STRING\n"); return NULL;
    }

    else
    {
        int a = atol( Car(input)->this.string );

        if(a == 0)
        {
            printf("String->Number : Ceci n'est pas un nombre\n");
            return (NULL);
        }
        return( make_integer(a) );
    }
}

/*
object symbolTostring(object input)
{
    if ( Car(input)->type != SFS_INTEGER )
    {
        printf("Char->integer : Operation impossible ceci n'est pas un SFS_CHARACTER\n"); return NULL;
    }

    else
    {
        return( make_integer( atoi( &(input->this.character) ) ) );
    }
}

object stringTosymbol(object input)
{
    if ( Car(input)->type != SFS_INTEGER )
    {
        printf("Char->integer : Operation impossible ceci n'est pas un SFS_CHARACTER\n"); return NULL;
    }

    else
    {
        return( make_integer( atoi( &(input->this.character) ) ) );
    }
}
*/