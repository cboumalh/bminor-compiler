#ifndef TYPECHECKUTILS_H
#define TYPECHECKUTILS_H

#include "type.h"

struct type * type_copy( struct type *t );
int type_compare( struct type *a, struct type *b );
void type_delete( struct type *t );


#endif