
/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"
#include "environement.h"
#include "object.h"
#include "primitive.h"

extern int VERB_SWITCH;

object sfs_eval( object input )
{
    /*
     Mise en mémoire de input d'origine pour ne pas perdre la tête :p
        object Tempi = input;
    Création d'un objet LNE (LastNeededEvalutaion) pour savoir quoi remplacer dès qu'on reussi à evaluer
        object LNE = input;
    Initialisation de HEAD_CHANGE pour savoir si l'on est dans la boucle principale ou dans une secondaire
        int HEAD_CHANGE = 0;*/

    /*return sup(input);*/
EVAL_in :

    switch(input->type)
    {
        case SFS_NIL :
            /* Auto-evaluant */
            return(nil);

        case SFS_PAIR :


            /*Cas_Primitives  */
            if ( Car(input)->type == SFS_SYMBOL )
            {
                object Temp1 = ENV_chercher(Car(input)->this.symbol);

                if ( Temp1->type == SFS_PRIMITIVE )
                {
                    return( Temp1->this.primitive.fonction(Cdr(input)) );
                }

                Temp1 = ENV_chercher( sfs_eval( Car(input) )->this.symbol );

                if ( Temp1->type == SFS_PRIMITIVE )
                {
                    return( Temp1->this.primitive.fonction(Cdr(input)) );
                }
            }




            /* cas DEFINE et SET! */
            if( !strcmp(Car(input)->this.symbol, "define") || !strcmp(Car(input)->this.symbol, "set!") )
            {
                if ( Car(Cdr(input))->type != SFS_SYMBOL )
                {
                    printf("ERREUR sfs_eval : seul les symboles peuvent être define ou set!\n");
                    exit(EXIT_FAILURE);
                }

                if ( !strcmp(Car(input)->this.symbol, "set!") && !ENV_est_defini(Car(Cdr(input))->this.symbol) )
                {
                    printf("ERREUR sfs_eval : Une variable non définie ne peut être set!\n");
                    exit(EXIT_FAILURE);
                }

                else
                {
                    ENV_definir( Car(Cdr(input))->this.symbol, Car(Cdr(Cdr(input))) );
                    /* Consigne de retour pas précise pour set! et define, on renvoie le symbole */
                    return(Car(Cdr(input)));
                }
            }


            /* cas IF */
            if( !strcmp(Car(input)->this.symbol, "if") )
            {
                if( Car(Cdr(input))->type != SFS_BOOLEAN )
                {
                    if ( Car(Cdr(input))->type !=SFS_PAIR )
                    {
                        input->this.pair.cdr->this.pair.car = T;
                    }

                    else
                        input->this.pair.cdr->this.pair.car = sfs_eval(Car(Cdr(input)));

                    goto EVAL_in ;
                }

                else
                {
                    if( Car(Cdr(input)) == T )
                    {
                        return(Car(Cdr(Cdr(input))));
                    }

                    else
                    {
                        if(Cdr(Cdr(Cdr(input)))->type == SFS_PAIR )
                        {
                            return(Car(Cdr(Cdr(Cdr(input)))));
                        }

                        else
                        {
                            return(F);
                        }
                    }
                }
            }

            /* cas AND */
            if( !strcmp(Car(input)->this.symbol, "and")  )
            {
                object Tmp1 = input;
                int Tmp2 = 0;

                while(Cdr(Tmp1)->type == SFS_PAIR)
                {
                    Tmp1 = Cdr(Tmp1);

                    if (Car(Tmp1)->type != SFS_BOOLEAN)
                    {
                        /* cas AND : Non Booleen */
                        if (VERB_SWITCH)
                            printf("cas AND : Non Booleen\n");

                        Tmp1->this.pair.car = sfs_eval(Car(Tmp1));
                    }

                    if ( Car(Tmp1) == T )
                    {
                        /* cas AND : #t Détécté */
                        Tmp2 = 1;
                    }

                    if ( Car(Tmp1) == F )
                    {
                        /* cas AND : #f Détécté */
                        return(F);
                    }
                }

                if ( Tmp2 == 0 )
                {
                    /* cas AND : Aucun #t */
                    printf("cas AND : Aucun #t détécté\n");
                    exit(EXIT_FAILURE);
                }

                else
                {
                    return(T);
                }
            }

            /* cas OR */
            if( !strcmp(Car(input)->this.symbol, "or") )
            {
                object Tmp1 = input;
                int Tmp2 = 1;

                while(Cdr(Tmp1)->type == SFS_PAIR)
                {
                    Tmp1 = Cdr(Tmp1);

                    if (Car(Tmp1)->type != SFS_BOOLEAN)
                    {
                        /* cas OR : Non Booleen */
                        if (VERB_SWITCH)
                            printf("cas OR : Non Booleen\n");

                        Tmp1->this.pair.car = sfs_eval(Car(Tmp1));
                        /*exit(EXIT_FAILURE);*/
                    }

                    if ( Car(Tmp1) == F )
                    {
                        /* cas OR : #f Détécté */
                        Tmp2 = 0;
                    }

                    if ( Car(Tmp1) == T )
                    {
                        /* cas OR : #t Détécté */
                        return(T);
                    }
                }

                if ( Tmp2 == 1 )
                {
                    /* cas OR : Aucun #f */
                    printf("cas OR : Aucun #f détécté\n");
                    exit(EXIT_FAILURE);
                }

                else
                {
                    return(F);
                }
            }

            printf("SFS_PAIR : Eval, Commande introuvable\n");
            exit(EXIT_FAILURE);
            break;

        case SFS_PRIMITIVE :
        {
            return(nil);
        }

        case SFS_SYMBOL :
        {

            object Obj1 = ENV_chercher(input->this.symbol);

            if(Obj1 == nil)
            {
                if (VERB_SWITCH)
                    printf("SFS_SYMBOL : Erreur Variable non définie\n");

                return(input);
            }

            else
            {
                return(Obj1);
            }
        }

        case SFS_INTEGER :
            /*Auto-evaluant*/
            return(input);

        case SFS_BOOLEAN :
            /*Auto-evaluant*/
            return(input);

        case SFS_CHARACTER :
            /*Auto-evaluant*/
            return(input);

        case SFS_STRING :
            /*Auto-evaluant*/
            return(input);

        default :
            printf("non evalué\n");
            return(nil);
            exit(EXIT_FAILURE);


    }
}