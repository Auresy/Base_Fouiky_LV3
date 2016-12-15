
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

object sfs_eval_Prim( object input, object EnvC )
{
    object Temp_List = input;
    while(Temp_List != nil)
    {
        object ErrDetect = sfs_eval( Car( Temp_List ), EnvC );
        if (ErrDetect == NULL) return(NULL);
        Temp_List->this.pair.car = ErrDetect ;
        Temp_List = Cdr(Temp_List);
    }
    return(input);
}



object sfs_eval( object input, object EnvC )
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


            /*Cas_symbol  */
            if ( Car(input)->type == SFS_SYMBOL )
            {
                object Temp1 = ENV_chercher(Car(input)->this.symbol, EnvC);

                /* Cas primitive */
                if ( Temp1 != NULL && Temp1->type == SFS_PRIMITIVE )
                {
                    object ErrChk = sfs_eval_Prim ( Cdr(input), EnvC );
                    if(ErrChk != NULL)
                    {
                        return( Temp1->this.primitive.fonction( sfs_eval_Prim ( Cdr(input), EnvC ) ) );
                    }
                    else return NULL;
                }
                /* cas compound */
                if ( Temp1 != NULL && Temp1->type == SFS_COMPOUND )
                {
                    Temp1 = sfs_eval( Temp1, EnvC );
                    /*printf("%d\n", Car(Cdr(input))->type );
                    printf("%d\n", Car(Cdr(input))->this.integer );*/
                    if (Cdr(input)->type == SFS_PAIR )
                    {
                        object Temp2 = make_pair( Temp1, Cdr(input) );
                        return sfs_eval( Temp2, EnvC );
                    }
                    else
                    {
                        printf ("Manque d'arguments");
                        return NULL;
                    }
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

                if ( Car(Cdr(input))->type == SFS_PAIR )
                /* Cas Define lambda OLDSCHOOL par transformations */
                {
                    object Nom_OS   = Car(Car(Cdr(input)));
                    object Args_OS  = Cdr(Car(Cdr(input)));
                    object Corps_OS = Car(Cdr(Cdr(input)));

                    object Tmp1_OS  = make_pair( Corps_OS, nil );
                    object Tmp2_OS  = make_pair( Args_OS, Tmp1_OS );
                    object Tmp3_OS  = make_pair( make_symbol("lambda"), Tmp2_OS );

                    input->this.pair.cdr->this.pair.car = Nom_OS;
                    input->this.pair.cdr->this.pair.cdr->this.pair.car = Tmp3_OS;

                    return( sfs_eval( input, EnvC ) );

                }

                if ( Car(Cdr(input))->type != SFS_SYMBOL )
                {
                    printf("ERREUR sfs_eval : Type, Seul les symboles peuvent être define ou set!\n");
                    return(NULL);
                }

                if ( !strcmp(Car(input)->this.symbol, "set!") && !ENV_est_defini(Car(Cdr(input))->this.symbol, EnvC) )
                {
                    printf("ERREUR sfs_eval : Type, Une variable non définie ne peut être set!\n");
                return(NULL);
                }

                else
                {
                    ENV_definir( Car(Cdr(input))->this.symbol, sfs_eval(Car(Cdr(Cdr(input))), EnvC), EnvC );
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
                        input->this.pair.cdr->this.pair.car = sfs_eval(Car(Cdr(input)), EnvC);

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

                        Tmp1->this.pair.car = sfs_eval(Car(Tmp1), EnvC);
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

                        Tmp1->this.pair.car = sfs_eval(Car(Tmp1), EnvC);
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
                    ret = sfs_eval(Car(input), EnvC);
                    input = Cdr( input );
                }
                return ret;
            }
            /* cas lambda : on crée un compound */
            if( !strcmp(Car(input)->this.symbol, "lambda") )
            {
                /* création d'un coumpound */
                return make_compound( Car(Cdr(input)), Car(Cdr(Cdr(input))), EnvC );

            }
            /* Cas LET : on transforme en lambda */
            if( !strcmp(Car(input)->this.symbol, "let") )
            {
                if ( Cdr(input)->type == SFS_PAIR && Cdr(Cdr(input))->type == SFS_PAIR )
                {
                    object corps = Car(Cdr(Cdr(input)));

                    object list_param_form=make_nil();
                    object list_param_eff=make_nil();

                    object list_arg = Car(Cdr(input));
                    if ( list_arg->type == SFS_PAIR ) 
                    {
                        while ( list_arg->type == SFS_PAIR )
                        {
                            if ( Car(list_arg)->type == SFS_PAIR && Cdr(Car(list_arg))->type == SFS_PAIR )
                            {
                                list_param_form = make_pair( Car(Car(list_arg)), list_param_form );
                                /*printf("%s\n", Car(list_param_form)->this.symbol );*/
                                list_param_eff = make_pair( Car(Cdr(Car(list_arg))), list_param_eff );
                                list_arg = Cdr(list_arg);
                            }
                            else 
                            {
                                printf("mauvaise synthaxe let");
                                return NULL;
                            }
                        }
                        object temp = make_pair( corps, make_nil() );
                        temp = make_pair( list_param_form, temp );
                        temp = make_pair( make_symbol("lambda"), temp);
                        temp = make_pair( temp, list_param_eff );
                        return sfs_eval(temp, EnvC);
                    }
                }
                printf("mauvaise synthaxe let");
                return NULL;
                
            }
            /* Cas PAIR : double pair donc un lambda compound*/
            if ( Car(input)->type == SFS_PAIR )
            {
                object compound = sfs_eval( Car(input), EnvC );
                object param = Cdr(input);
                object t=make_pair(compound, param);

                /* verif des args */
                object list_param_eff = Cdr(t);
                object list_param_form = Car(t)->this.compound.param;
                while ( list_param_form->type == SFS_PAIR && list_param_eff->type == SFS_PAIR )
                {
                    /* verifier que l'on a bien des paramètres formels (symbols) */
                    if ( Car(list_param_form)->type != SFS_SYMBOL )
                    {
                        /*printf("%d", Car(list_param_form)->type);
                        printf("%d", Car(list_param_form)->this.integer);*/
                        printf("Le premier argument d'un compound est un(des) paramètre(s) (symbol)\n");
                        return NULL;
                    }
                    list_param_eff = Cdr(list_param_eff);
                    list_param_form = Cdr(list_param_form);
                }
                if ( list_param_form->type == SFS_PAIR || list_param_eff->type == SFS_PAIR )
                {
                    printf("Le compound n'a pas le bon nombre d'arguments\n");
                    return NULL;
                }

                /* création d'un environnement local */
                object EnvN;
                EnvN=ENV_NewEnv( ENV_TETE );


                /* association de l'évaluation du paramètre effectif au paramètre formel */
                object ret_param_form = Car(t)->this.compound.param;
                object ret_param_eff = Cdr(t);
                    while ( ret_param_form->type == SFS_PAIR && ret_param_eff->type == SFS_PAIR)
                    {
                        ENV_definir( Car(ret_param_form)->this.symbol, Car(ret_param_eff), EnvN); 
                        ret_param_form = Cdr(ret_param_form);
                        ret_param_eff = Cdr(ret_param_eff);
                    }

                /* exec du corps de la fonction */

                /*? EnvC = EnvN; ?*/
                return sfs_eval( Car(t)->this.compound.body, EnvN );

                printf("Il manque des arguments a la forme lambda\n");
                return(NULL);
            }
            /* cas compound */
            if( Car(input)->type == SFS_COMPOUND && Cdr(input)->type == SFS_PAIR )
            {
                /* verif des args */
                object list_param_eff = Cdr(input);
                object list_param_form = Car(input)->this.compound.param;
                while ( list_param_form->type == SFS_PAIR && list_param_eff->type == SFS_PAIR )
                {
                    /* verifier que l'on a bien des paramètres formels (symbols) */
                    if ( Car(list_param_form)->type != SFS_SYMBOL )
                    {
                        /*printf("%d", Car(list_param_form)->type);
                        printf("%d", Car(list_param_form)->this.integer);*/
                        printf("Le premier argument d'un compound est un(des) paramètre(s) (symbol)\n");
                        return NULL;
                    }
                    list_param_eff = Cdr(list_param_eff);
                    list_param_form = Cdr(list_param_form);
                }
                if ( list_param_form->type == SFS_PAIR || list_param_eff->type == SFS_PAIR )
                {
                    printf("Le compound n'a pas le bon nombre d'arguments\n");
                    return NULL;
                }

                /* création d'un environnement local */
                object EnvN;
                EnvN=ENV_NewEnv( ENV_TETE );


                /* association de l'évaluation du paramètre effectif au paramètre formel */
                object ret_param_form = Car(input)->this.compound.param;
                object ret_param_eff = Cdr(input);
                    while ( ret_param_form->type == SFS_PAIR && ret_param_eff->type == SFS_PAIR)
                    {
                        ENV_definir( Car(ret_param_form)->this.symbol, Car(ret_param_eff), EnvN); 
                        ret_param_form = Cdr(ret_param_form);
                        ret_param_eff = Cdr(ret_param_eff);
                    }

                /* exec du corps de la fonction */

                /*? EnvC = EnvN; ?*/
                return sfs_eval( Car(input)->this.compound.body, EnvN );

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

            object Obj1 = ENV_chercher(input->this.symbol, EnvC);

            if(Obj1 == NULL)
            {
                if (1)
                {
                    printf("SFS_SYMBOL : Erreur Variable non définie  => %s\n", input->this.symbol);
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

        case SFS_COMPOUND :
            /*Auto-evaluant*/
            return(input);

        default :
            printf("Eval.c : FATAL non evaluable\n");
            exit(EXIT_FAILURE);


    }
}