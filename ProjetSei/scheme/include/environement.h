#include "object.h"


object ENV_maillon(); /*Alloue un maillon pour l'environement*/
object ENV_NewEnv(object papa);
void ENV_init(); /*Initialise les variables d'environement SCHEME et la globale object* "ENV_TETE"*/
void ENV_definir(char* Symbole, object Obj, object EnvC); /*Associe une chaine "Symbole" à un objet*/
void ENV_redefinir(char* Symbole, object Obj, object EnvC); /*Supprime et redefini <=> ENV_supprimer + ENV_defnir*/
int ENV_est_defini(char* Symbole, object EnvC); /*Renvoie 1 si la chaîne Symbole est deja utilisée 0 sinon*/
object ENV_chercher(char* Symbole, object EnvC); /*Renvoie l'objet associé à la chaîne Symbole s'il existe et NULL sinon*/
void ENV_supprimer(char* Symbole); /*Supprime et libère la memoire pour le symbole donné*/
void ENV_purifier(); /*libère l'environement en supprimant toutes les variables puis en detruisant la tête*/
void ENV_List( object EnvC ); /*Liste les variables déclarées dans l'environnement*/