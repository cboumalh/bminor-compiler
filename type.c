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
    if (t->kind == TYPE_VOID)
        printf("void ");
    else if (t->kind == TYPE_BOOLEAN)
        printf("boolean ");
    else if (t->kind == TYPE_CHARACTER)
        printf("char ");
    else if (t->kind == TYPE_INTEGER)
        printf("integer ");
    else if (t->kind == TYPE_STRING)
        printf("string ");
    else if (t->kind == TYPE_AUTO)
        printf("auto ");
    else if (t->kind == TYPE_FLOAT)
        printf("float ");
	else if (t->kind == TYPE_ARRAY) {
        printf("array [] ");
        type_print(t->subtype);
	}
	else if (t->kind == TYPE_FUNCTION){
        printf("function ");
		type_print(t->subtype);
		printf("( ");
		param_list_print(t->params);
		printf(") ");
	}

}
