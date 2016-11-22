#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_
#include "object.h"

void creation_librairie(void);

object add(object input);
object sub(object input);
object multiplie(object input);
object quotient(object input);
object remainder(object input);
object equal(object input);
object sup(object input);
object inf(object input);

object predicatnull(object input);
object predicatboolean(object input);
object predicatsymbol(object input);
object predicatinteger(object input);
object predicatchar(object input);
object predicatstring(object input);
object predicatpair(object input);
object predicatprocedure(object input);

object cons(object input);
object primitivecar(object input);
object primitivecdr(object input);
object setcar(object input);
object setcdr(object input);
object list(object input);

object qtype(object input);
object charToint(object input);
object intTochar(object input);
object numTostring(object input);

object stringTonum(object input);
/*
object symbolTostring(object input);
object stringTosymbol(object input);
*/


#endif /* _PRIMITIVE_H_ */
