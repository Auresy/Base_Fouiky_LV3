#include "decoupe.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* decoupe(char* input, unsigned int Nbin, unsigned int Nbout)
{

	/* Allocation tableau char - attention au \0 */
	char* chaine = NULL;
	chaine = calloc(256, sizeof(char) );
	if ( chaine == NULL ) {
		printf("ALLOC MEMOIRE malloc MISS\n");
		exit(EXIT_FAILURE);
	}

	for (int i = Nbin; i < Nbout; ++i)
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
	while (lettre != sep)
	{
		Nbin++;
	}
}

