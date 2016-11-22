#include "decoupe.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char* str=NULL;
	str = calloc(256, sizeof(char));
	//str[254]= '\0';
	if (str==NULL)
	{
		printf("FATAL MEM ALLOC\n");
		exit(EXIT_FAILURE);
	}
	

	printf("texte a saisir\n");
	str = "Espace Espace Espace";
	printf("entree sortie int\n");
	int i,j;
	scanf("%d %d", &i, &j);
	printf("%d %d %s\n", i,j,str );
	// i = detect(str, i, ' ');
	// printf("%d %d %s\n", i,j,str );
	
	str = decoupe(str, i, j);
	printf("%d %d %s\n", i,j,str );
	
	char c = '9';
	printf("%d\n", c );

	return 0;

}