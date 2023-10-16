#include "type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params ){
	struct type * t = calloc(1, sizeof(struct type));

	memset(t, 0, sizeof(struct type));

	t->kind = kind;
	t->subtype = subtype;
	t->params = params;

	return t;
}

void type_print( struct type *t ){
    return;
}