
char* decoupe(char* input, unsigned int Nbin, unsigned int Nbout); 
/*renvoie le découpage d'une châîne input entre unsigned int1 et unsigned int2 */
void free_decoupe(char* chaineDecoupe);
/* libere la memoire fct decoupe */



unsigned int detect(char* input, unsigned int Nbin, char sep);
/* Renvoie la position de la prochaine occurence de "sep" en partant de Nbin */

