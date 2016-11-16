#include "decoupe.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* decoupe(char* input, unsigned int Nbin, unsigned int Nbout)
{

    /* Allocation tableau char - attention au \0 */
    char* chaine = NULL;
    chaine = calloc(256, sizeof(char) );

    if ( chaine == NULL )
    {
        printf("ALLOC MEMOIRE malloc MISS\n");
        exit(EXIT_FAILURE);
    }

    int i=0;

    for (i = Nbin; i < Nbout; i++)
    {
        chaine[i - Nbin]=input[i];
    }

    return(chaine);
}

void free_decoupe(char* chaineDecoupe)
{
    free(chaineDecoupe);
}

unsigned int detect(char* input, unsigned int Nbin, char sep)
{
    int len = strlen(input);
    char lettre = input[Nbin];

    if ( sep != '"' )
    {
        while (lettre != sep && Nbin < len && (int)lettre != 9 && lettre != '(' && lettre != ')')
        {
            Nbin++;
            lettre = input[Nbin];
        }

        if ( Nbin==len && input[Nbin-1] == ')')
        {
            return(Nbin-1);
        }
    }

    else
    {
        while (lettre != sep && Nbin < len )
        {
            Nbin++;
            lettre = input[Nbin];
        }

        if ( input[Nbin-1] == '\\' )
        {
            return(Nbin-1);
        }


    }

    return(Nbin);
}

