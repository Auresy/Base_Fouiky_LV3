
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
extern object ENV_TETE;

object sfs_eval_Prim( object input )
{
    object Temp_List = input;
    while(Temp_List != nil)
    {
        object ErrDetect = sfs_eval( Car( Temp_List ) );
        if (ErrDetect == NULL) return(NULL);
        Temp_List->this.pair.car = ErrDetect ;
        Temp_List = Cdr(Temp_List);
    }
    return(input);
}



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

                if ( Temp1 != NULL && Temp1->type == SFS_PRIMITIVE )
                {
                    return( Temp1->this.primitive.fonction( sfs_eval_Prim ( Cdr(input)) ) );
                }
                if (Temp1 != NULL)
                Temp1 = ENV_chercher( sfs_eval( Car(input) )->this.symbol );

                if ( Temp1 != NULL && Temp1->type == SFS_PRIMITIVE )
                {
                    return( Temp1->this.primitive.fonction(sfs_eval_Prim( Cdr(input) ) ) );
                }
            }

             /* cas Quote ou ' */
            if( !strcmp(Car(input)->this.symbol, "quote"))
            {
                return(Car(Cdr(input)));
            }

            /* cas DEFINE et SET! */
            if( !strcmp(Car(input)->this.symbol, "define") || !strcmp(Car(input)->this.symbol, "set!") )
            {
                if( Cdr(input) == nil || Cdr(Cdr(Cdr(input))) != nil )
                {
                    printf("ERREUR sfs_eval : Arguments define-set! incorrects\n");
                    return(NULL);
                   
                }
                if ( Car(Cdr(input))->type != SFS_SYMBOL )
                {
                    printf("ERREUR sfs_eval : Type, Seul les symboles peuvent être define ou set!\n");
                    return(NULL);
                }

                if ( !strcmp(Car(input)->this.symbol, "set!") && !ENV_est_defini(Car(Cdr(input))->this.symbol) )
                {
                    printf("ERREUR sfs_eval : Type, Une variable non définie ne peut être set!\n");
                return(NULL);
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
                        input = Car(Cdr(Cdr(input)));
                    }

                    else
                    {
                        if(Cdr(Cdr(Cdr(input)))->type == SFS_PAIR )
                        {
                            input = Car(Cdr(Cdr(Cdr(input))));
                        }

                        else
                        {
                            input = F;
                        }
                    }
                    goto EVAL_in ;
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
            /* cas begin */
            if( !strcmp(Car(input)->this.symbol, "begin") )
            {
                object ret=make_nil();
                input = Cdr(input);
                while ( input->type != SFS_NIL )
                {
                    ret = sfs_eval(Car(input));
                    input = Cdr( input );
                }
                return ret;
            }
            /* cas lambda */
            if( !strcmp(Car(input)->this.symbol, "lambda") )
            {
                /* création d'un coumpound */
                return make_compound( Car(Cdr(input)), Car(Cdr(Cdr(input))), ENV_TETE );

            }
            /* Cas PAIR */
            if ( Car(input)->type == SFS_PAIR )
            {
                object t = sfs_eval( Car(input) );
                object t2 = sfs_eval( Cdr(input) );
                return sfs_eval(make_pair(t, t2));
            }
            /* Cas compound */
            if ( Car(input)->type == SFS_COMPOUND)
            {
                /* verif des args */
                object ret1 = Cdr(input);
                object ret2 = Car(input)->this.compound.param;
                while ( ret1->type != SFS_PAIR && ret1->type != SFS_PAIR)
                {
                    /* verifier que l'on a bien des paramètres formels (symbols) */
                    if ( ret1->type != SFS_SYMBOL )
                    {
                         printf("Le premier argument de la forme lambda est un(des) paramètre(s) (symbol)");
                        return NULL;
                      }
                    ret2 = Cdr(ret2);
                    ret1 = Cdr(ret1);
                }

            

                /* création d'un environnement local */

                /* association de l'évaluation du paramètre effectif au paramètre formel */
                object ret_param_form = Car(Cdr(Car(input)));
                object ret_param_eff = Cdr(input);
                    while ( ret_param_form->type != SFS_PAIR && ret_param_eff->type != SFS_PAIR)
                    {
                        ENV_definir( Car(ret_param_form)->this.symbol, Car(ret_param_eff)); /* a modifier avec l'ENV*/
                        ret_param_form = Cdr(ret_param_form);
                        ret_param_eff = Cdr(ret_param_eff);
                    }
                /* exec du corps de la fonction */

                printf("Il manque des arguments a la forme lambda\n");
                return(NULL);
            }
            printf("SFS_PAIR : Eval, Commande introuvable\n");
            return(NULL);
            break;

        case SFS_PRIMITIVE :
        {
            return(nil);
        }

        case SFS_SYMBOL :
        {

            object Obj1 = ENV_chercher(input->this.symbol);

            if(Obj1 == NULL)
            {
                if (1)
                {
                    printf("SFS_SYMBOL : Erreur Variable non définie\n");
                    return(NULL);
                }
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
            printf("Eval.c : FATAL non evaluable\n");
            exit(EXIT_FAILURE);


    }
}
