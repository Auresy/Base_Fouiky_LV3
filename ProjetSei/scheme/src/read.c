
/**
 * @file read.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:31:05 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#include <stdio.h>
#include <ctype.h>

#include "decoupe.h"
#include "read.h"



void flip( uint *i )
{

    if ( *i == FALSE )
    {
        *i = TRUE;
    }

    else
    {
        *i = FALSE;
    }
}

/*
 * @fn char* first_usefull_char(char* line)
 *
 * @brief retourne un pointeur sur le premier caractere utile dans line
 * ou NULL si line ne contient que des espaces et des commentaires
 */
char* first_usefull_char(char* line)
{

    int i=0;

    if (line == NULL)
    {
        return NULL;

    }

    i = 0;

    /* on saute les espaces */
    while(line[i] != '\0' && isspace(line[i]))
    {
        i++;
    }

    /* si fin de ligne => ligne inutile */
    if(line[i] == '\0')
    {
        return NULL;
    }

    /* si premier caractere non espace est ';' => ligne inutile */
    if(line[i] == ';')
    {
        return NULL;
    }

    return line + i; /* ligne utile */
}

/**
 * @fn uint  sfs_get_sexpr( char *input, FILE *fp )
 *
 * @brief extrait la prochaine S-Expression dans le flux fp et la stocke dans input
 * (meme si elle est repartie sur plusieurs lignes)
 * @param fp (FILE *) flux d'entree (ouvert en ecriture, mode texte)
 * @param input (char *) chaine allouee de taille BIGSTRING, dans laquelle la S-Expression sera stockée
 *
 * @return S_OK si une S-Expression apparemment valide a ete trouvee
 * @return S_KO si on n'a pas trouve de S-Expression valide
 * @return S_END si fin de fichier atteinte sans avoir lu de caractere utile.
 *
 * sfs_get_sexpr commence par lire une ligne dans fp,
 * puis compte le nombre de parentheses ouvrantes et fermantes sur la ligne.
 * Les parentheses dans des chaines et les caracteres Scheme #\( et #\)
 * ne sont pas comptes.
 *
 * Si le compte devient zéro et que
 *        - la ligne est fini, la fonction retourne S_OK
 *              - la ligne n'est pas fini la fonction retourne S_KO
 *
 * S'il y a plus de parentheses fermantes qu'ouvrantes,
 * la fonction retourne S_KO.
 * Les commentaires et espaces qui figurent a la fin de chaque ligne
 * sont remplacés par un espace.
 * Les espaces qui figurent a la fin de la S-Expression (y compris '\n')
 * sont supprimés.
 *
 * Attention : cette fonction refuse les S-Expression multiple sur une seule ligne. Ainsi :
 *    a b c
 *    (qqchose) (autrechose)
 *    (qqchose) 78
 * seront considereees comme des erreurs
 * et la fonction retournera S_KO
 *
 * @pre fp doit etre prealablement ouvert en lecture
 * @pre input doit etre prealablement alloue en memoire, de taille BIGSTRING
 */

typedef enum
{
    NOTHING,        /* rien n'a ete trouve encore.. */
    STRING_ATOME,   /* la premiere trouvee dans la ligne semble etre un atome */
    BASIC_ATOME,    /* la premiere trouvee dans la ligne semble etre d'une chaine */
    S_EXPR_PARENTHESIS, /* la premiere trouvee dans la ligne semble etre une expression parenthesee */
    FINISHED        /* on a trouve une S-Expr bien formee */
} EXPRESSION_TYPE_T;

uint  sfs_get_sexpr( char *input, FILE *fp )
{
    int       parlevel = 0;
    uint      in_string = FALSE;
    uint      s = 0;
    char      k[BIGSTRING];
    char      *ret = NULL;
    char      *chunk = NULL;
    uint      i = 0;
    string    sfs_prompt;

    EXPRESSION_TYPE_T typeOfExpressionFound = NOTHING;

    parlevel = 0;
    memset( input, '\0', BIGSTRING );

    do
    {
        ret = NULL;
        chunk=k;
        memset( chunk, '\0', BIGSTRING );

        /* si en mode interactif*/
        if ( stdin == fp )
        {
            uint nspaces = 2*parlevel;

            init_string( sfs_prompt );

            /* le prompt indique le niveau de parenthese
               et decale la prochaine entrée en fonction
               de ce niveau (un peu à la python)*/
            sprintf( sfs_prompt, "SFS:%u > ", parlevel );

            for ( i= 0; i< nspaces; i++ )
            {
                sfs_prompt[strlen(sfs_prompt)] = ' ';
            }

            /* si sur plusieurs lignes, le \n équivaut à un espace*/
            if (nspaces>0)
            {
                input[strlen(input)+1] = '\0';
                input[strlen(input)] = ' ';
            }

            /*saisie de la prochaine ligne à ajouter dans l'input*/
            printf("%s",sfs_prompt);
            ret = fgets( chunk, BIGSTRING, fp );

            if (ret && chunk[strlen(chunk)-1] == '\n') chunk[strlen(chunk)-1] = '\0';

        }

        /*si en mode fichier*/
        else
        {
            ret = fgets( chunk, BIGSTRING, fp );

            if ( NULL == ret )
            {
                /* fin de fichier...*/
                if ( parlevel != 0 )
                {
                    WARNING_MSG( "Parse error: missing ')'" );
                    return S_KO;
                }

                return S_END;
            }

            if (strlen(chunk) == BIGSTRING-1
                    && chunk[BIGSTRING-1] != '\n'
                    && !feof(fp))
            {
                WARNING_MSG( "Too long line for this interpreter!" );
                return S_KO;
            }
        }

        /* si la ligne est inutile
            => on va directement à la prochaine iteration */
        if (first_usefull_char(chunk) == NULL)
        {
            continue;
        }


        s = strlen( chunk );

        if ( s > 0 )
        {
            if (strlen(input) + s > BIGSTRING-1 )
            {
                WARNING_MSG( "Too long a S-expression for this interpreter!" );
                return S_KO;
            }

            for ( i = 0; i< strlen(chunk); i++ )
            {
                /* si la fin de la ligne chunk est inutile,
                   on ajoute un espace dans input et on sort de la boucle*/
                if ( in_string == FALSE && first_usefull_char(chunk + i) == NULL )
                {
                    chunk[i]='\0';
                    input[strlen(input)] = ' ';
                    break;
                }


                switch(chunk[i])
                {
                    case '(':
                        if (in_string == FALSE
                                && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) )
                        {
                            parlevel++;
                            typeOfExpressionFound = S_EXPR_PARENTHESIS;
                        }

                        break;

                    case ')':
                        if ( in_string == FALSE
                                && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) )
                        {
                            parlevel--;

                            if (parlevel == 0 )
                            {
                                typeOfExpressionFound = FINISHED;
                            }

                            if ( parlevel < 0 )
                            {
                                WARNING_MSG( "Parse error : cannot start with ')'" );
                                return S_KO;
                            }
                        }

                        break;

                    case '"':
                        if ( i<2 || chunk[i-1] != '\\' )
                        {
                            if ( in_string == FALSE )
                            {
                                if(typeOfExpressionFound == BASIC_ATOME)
                                {
                                    WARNING_MSG("Parse error: invalid string after atom : '%s'", chunk+i);
                                    return S_KO;
                                }

                                in_string = TRUE;

                                if(typeOfExpressionFound != S_EXPR_PARENTHESIS)
                                {
                                    typeOfExpressionFound = STRING_ATOME;
                                }
                            }

                            else
                            {
                                in_string = FALSE;

                                if(typeOfExpressionFound == STRING_ATOME)
                                {
                                    typeOfExpressionFound = FINISHED;
                                }
                            }
                        }

                        break;

                    default:
                        if(in_string == FALSE)
                        {
                            if(isspace(chunk[i]))
                            {
                                if(typeOfExpressionFound == BASIC_ATOME)
                                {
                                    typeOfExpressionFound = FINISHED;
                                }
                            }

                            else
                                if(typeOfExpressionFound != S_EXPR_PARENTHESIS && chunk[i] != '\'')
                                {
                                    typeOfExpressionFound = BASIC_ATOME;
                                }
                        }

                        break;
                }


                if(typeOfExpressionFound == FINISHED)
                {
                    char *first_useful = first_usefull_char(chunk + i + 1);

                    if( first_useful != NULL)
                    {
                        if(*first_useful == ')' )
                        {
                            WARNING_MSG( "Parse error: too many closing parenthesis')'" );
                        }

                        else
                        {
                            WARNING_MSG("Parse error: invalid trailing chars after S-Expr : '%s'", chunk+i);
                        }

                        return S_KO;
                    }
                }

                /* recopie char par char*/
                input[strlen(input)] = chunk[i];
            }

            if(in_string == TRUE)
            {
                WARNING_MSG( "Parse error: non terminated string on line %s", chunk );
                return S_KO;
            }
        }


        if ( parlevel > 0 && fp != stdin )
        {
            if ( feof( fp ) )
            {
                WARNING_MSG( "Parse error: missing ')'" );
                return S_KO;
            }

            if (input[strlen(input)-1] == '\n') input[strlen(input)-1] = ' ';
        }
    }
    while ( parlevel > 0 );

    /* Suppression des espaces restant a la fin de l'expression, notamment le dernier '\n' */
    while (isspace(input[strlen(input)-1])) input[strlen(input)-1] = '\0';

    return S_OK;
}


object sfs_read( char *input, uint *here )
{
    Space_Skip(input, here);

    if ( input[*here] == '(' )
    {
        (*here)++;
        Space_Skip(input, here);

        if ( input[(*here)] == ')' )
        {
            (*here)++;
            return nil;
        }

        else
        {
            return sfs_read_pair( input, here );
        }
    }

    else
    {
        return sfs_read_atom( input, here );
    }
}

/* Le but de cette fonction va être de choisir quelle fonction sous fonction de read_atom appeler en fonction du premier caractere
de la chaine. Les fonctions peuvent se renvoyer l'atome si elles s'apercoivent qu'il ne leur est pas destiné. */

object sfs_read_atom( char *input, uint *here )
{
    switch(input[*here])
    {
        case ' ' : /*printf("BUG DE L'ESPACE\n");*/
            (*here)++;
            return sfs_read( input, here);
            break;

        case '#' :
            if((int)input[*here+1] == 92)
            {
                return sfs_read_character( input, here );
                break;
            }

            else
            {
                return sfs_read_boolean(input, here );    /* le readbolean peut renvoyer symbol si suite */
                break;
            }

        case '"' :
            return sfs_read_string( input, here );
            break;

        case '\'' :
            (*here)++;
            return sfs_read_quote( input, here );
            break;

        default :

            /* saute espace */
            if ( (int)input[*here] == 32 || (int)input[*here]==9 )
            {
                (*here)++;
                return sfs_read( input, here);
                break;
            }

            /* cas nombre */
            if ( (int)input[*here]==43 || (int)input[*here]==45 || (48 <=(int)input[*here] && (int)input[*here]<= 57 ))
            {
                return sfs_read_integer( input, here );
            }

            /* cas symbole */
            else
                if ( (int)input[*here]==33  ||  (42<=(int)input[*here] && (int)input[*here]<=126 ) ||  (35<=(int)input[*here] && (int)input[*here]<=38 ))
                {
                    return sfs_read_symbol( input, here );
                }

                /* cas erreur */
                else
                {
                    printf("ERREUR read_atom, verifiez la syntaxe\n");
                    return NULL;
                }
    }

}

object sfs_read_quote(char *input, uint *here)
{
    object car=make_symbol("quote");
    object cdr=sfs_read(input, here);
    object t=make_pair(car,cdr);
    return t;
}

/* Cette sous fonction de read_atom vérifie que l'atome reçu est bien un booléen, si ce n'est pas le cas, renvoie vers read_symbol
 ou vers une erreur. Elle retourne un pointeur sur l'object true ou sur l'object false en fonction de l'entrée.
 A la fin, le pointeur here pointe sur la suite de input.*/

object sfs_read_boolean(char *input, uint *here )
{
    if ( input[*here] == '#' )
    {
        if ( input[*here+2] == '(' || input[*here+2] == ')' || input[*here+2] == ' ' || (int)input[*here+2] == 00)
        {
            if ( input[*here+1] == 't')
            {
                *here+=2; /* here passe à l'atome/pair suivant */
                /*printf ("%c", input[*here]);*/
                return T;
            }

            else
                if (input[*here+1] == 'f')
                {
                    *here+=2; /* here passe à l'atome/pair suivant */
                    return F;
                }

                else
                {
                    /*printf("ERREUR read boolean");*/ return NULL;
                }
        }

        else
        {
            /*printf("ERREUR read boolean");*/ return NULL;
        }
    }

    else
    {
        /*printf("ERREUR read boolean");*/ return NULL;
    }

}

/* Cette sous fonction de read_atom vérifie que l'atome reçu est bien un character, et renvoie un object character dépendant de l'input.
A la fin, le pointeur here pointe sur la suite de input.*/


object sfs_read_character( char *input, uint *here )
{
    if ( input[*here+3] == '(' || input[*here+3] == ')' || input[*here+3] == ' ' || (int)input[*here+3] == 0 )
    {
        object t=make_character((input[*here+2]));
        *here+=3;
        return t;
    }

    else
        if (!(strcmp(decoupe(input, *here+2, ' '), "space")))   /* comme on peut pas stocker space dans un caractère on stocke la chaine "#\space" dans les symboles*/
        {
            object t=make_symbol("#\\space");
            *here+=7;
            return t;
        }

        else
            if (!(strcmp(decoupe(input, *here+2, ' '), "newline")))   /* comme on peut pas stocker space dans un caractère on stocke la chaine "#\newline" dans les symboles*/
            {
                object t=make_symbol("#\\newline");
                *here+=9;
                return t;
            }

    /*printf("ERREUR read character, verifiez votre syntaxe\n");*/ return NULL;
}

object sfs_read_string( char *input, uint *here )  /*on risque d'avoir des pbs de taille a cause de la concaténation*/
{
    if (input[*here] == '"' &&  ( input[detect(input, *here+1, '"')] == '"' || input[detect(input, *here+1, '"')] == '\\' ) )
    {
        object t=make_string(decoupe(input, *here+1, detect(input, *here+1, '"')));
        *here=detect(input, *here+1, '"')+1;

        if ( input[(*here)-1] == '\\' )
        {
            char s[BIGSTRING];
            strcat(s, decoupe(input, *here, detect(input, *here+1, '"')));
            strcat(t->this.string, s);
        }

        /*printf ("%s", t->this.string);*/
        return t;
    }

    else
    {
        printf("ERREUR read string, verifiez votre syntaxe\n");
        return NULL;
    }
}


/* Cette sous fonction de read_atom vérifie que l'atome reçu est bien un integer, et renvoie un object integer dépendant de l'input,
grâce aux fonctions decoupe & dectect.
A la fin, le pointeur here pointe sur la suite de input.*/

object sfs_read_integer( char *input, uint *here )
{
    int i=0;

    if ( ( input[*here] == '+' || input[*here] == '-' ) && ( input[(*here)+1] == ' ' || input[(*here)+1] == ')' || input[(*here)+1] == '(' || (int)input[(*here)+1] == 9 || (int)input[(*here)+1] == 0) )
    {
        return sfs_read_symbol( input, here );
    }

    if ( ( input[*here] == '+' || input[*here] == '-' ) && ( input[*here+1] == '+' || input[*here+1] == '-' ) )
    {
        printf("ERREUR read integer, verifiez votre syntaxe\n");
        return NULL;
    }

    while ( !(input[( *here )+i] == ' ') && (*here + i) < (int)strlen(input) && !(input[( *here )+i] == ')'))
    {
        if ( (int)input[( *here )+i]==43 || (int)input[( *here )+i]==45 || (48 <=(int)input[( *here )+i] && (int)input[( *here )+i]<= 57 ) )
        {
            i+=1;
        }

        /*else if ((int)input[( *here )+i]==33  ||  (42<=(int)input[( *here )+i] && (int)input[( *here )+i]<=125 ) ||  (35<=(int)input[( *here )+i] && (int)input[( *here )+i]<=38 ) ){
            return sfs_read_symbol( input, here );
        }*/
        else
        {
            printf("ERREUR read integer, verifiez votre syntaxe\n");
            return NULL;
        }
    }

    object t=make_integer(atoi(decoupe(input, *here, detect(input, *here, ' '))));
    /*printf("%d\n", t->this.integer);*/
    *here+=i;
    return t;
}



object sfs_read_symbol( char *input, uint *here )
{
    unsigned int i=detect(input, *here, ' ');
    int a=0;

    while ( !(input[*here+a] == ' '|| (*here + a)< strlen(input) ))
    {
        if ( (int)input[*here+a]==34 )
        {
            printf("ERREUR read symbol, verifiez votre syntaxe\n");
            return NULL;
        }

        a+=1;
    }

    object t=make_symbol(decoupe(input, *here, i));
    *here=i;

    return t;
}


object sfs_read_pair( char *input, uint *here )
{
    object car;
    object cdr;

    Space_Skip(input, here);
    car=sfs_read(input, here);
    Space_Skip(input, here);

    if ( input[(*here)] == ')' )
    {
        cdr=nil;
        (*here)++;
    }

    else
    {
        cdr=sfs_read_pair(input, here); /*here a avancé dans le read_atom donc il pointe mtn sur le cdr*/
        Space_Skip(input, here);
    }

    object pair = make_pair(car, cdr);
    return pair;
}

void Space_Skip(char *input, unsigned int *here)
{
    while ( (int)input[*here] == 32 || (int)input[*here]==9)  /*on passe les espaces/tab superflux s'il y en a*/
    {
        (*here)++;
    }
}

