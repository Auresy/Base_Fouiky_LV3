
/**
 * @file read.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:30:11 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#ifndef _READ_H_
#define _READ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "object.h"

enum {S_OK, S_KO, S_END};

uint   sfs_get_sexpr( string input, FILE *fp );

object sfs_read( char *input, uint *here );
object sfs_read_atom( char *input, uint *here );
object sfs_read_pair( char *stream, uint *i );
object sfs_read_boolean(char *input, uint *here );
object sfs_read_character( char *input, uint *here );
object sfs_read_string( char *input, uint *here );
object sfs_read_integer( char *input, uint *here );
object sfs_read_symbol( char *input, uint *here );
object sfs_read_quote( char *input, uint *here );

object Car(object);
object Cdr(object);

void Space_Skip(char *input, uint *here);



#ifdef __cplusplus
}
#endif

#endif /* _READ_H_ */
