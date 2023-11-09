#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"
#include "expr.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
	TYPE_AUTO,
	TYPE_FLOAT
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
	struct expr *size;
	struct type *next;
};

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params, struct expr *size, struct type *next);
void          type_print( struct type *t );
struct type * type_copy( struct type *t );
int type_equals( struct type *a, struct type *b );
void type_delete( struct type *t );

#endif
