#include "type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params, struct expr *size, struct type *next){
	struct type * t = calloc(1, sizeof(struct type));

	memset(t, 0, sizeof(struct type));

	t->kind = kind;
	t->subtype = subtype;
	t->params = params;
    t->size = size;
    t->next = next;

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
        printf("array [");
        expr_print(t->size);
        printf("] ");
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

struct type * type_copy( struct type *t ){
    if(!t) return t;

    struct type * cpy = calloc(1, sizeof(struct type));
	memset(cpy, 0, sizeof(struct type));

	cpy->kind = t->kind;
	cpy->subtype = type_copy(t->subtype);
	cpy->params = param_copy(t->params);
    cpy->size = expr_copy(t->size);
    cpy->next = type_copy(t->next);

	return cpy;
}

void type_delete( struct type *t ){
    if(!t) return;
    param_delete(t->params);
    expr_delete(t->size);
    type_delete(t->subtype);
    type_delete(t->next);

    free(t);
}

int type_equals(struct type *a, struct type *b){
    if(!a || !b) return 0;
    if(a->kind != b->kind) return 0;

    if(a->kind == TYPE_ARRAY)
        return type_equals(a->subtype, b->subtype);

    else if(a->kind == TYPE_FUNCTION)
        return type_equals(a->subtype, b->subtype) && param_list_equals(a->params, b->params);

    return 1;
}