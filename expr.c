#include "expr.h"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ){
	struct expr * e = calloc(1, sizeof(struct expr));

	memset(e, 0, sizeof(struct expr));

	e->kind = kind;
	e->left = left;
	e->right = right;

	return e;
}

struct expr * expr_create_name( const char *n ){
	struct expr * e;

	char *temp =  strdup(n); 

	e = expr_create(EXPR_NAME, NULL, NULL);
	e->name = temp;

	return e;
}

struct expr * expr_create_integer_literal( int c ){
	struct expr * e;

	e = expr_create(EXPR_INTEGER_LITERAL, NULL, NULL);
	e->literal_value = c;

	return e;

}

struct expr * expr_create_boolean_literal( int c ){
	struct expr * e;

	e = expr_create(EXPR_BOOL_LITERAL, NULL, NULL);
	e->literal_value = c;

	return e;

}

struct expr * expr_create_char_literal( char *c ){
	struct expr * e;

	e = expr_create(EXPR_CHAR_LITERAL, NULL, NULL);
	e->literal_value = (int) strtochar(c);

	return e;

}

struct expr * expr_create_string_literal( const char *str ){
	struct expr * e;

	e = expr_create(EXPR_STRING_LITERAL, NULL, NULL);
	e->string_literal = strdup(str);

	return e;
}

struct expr * expr_create_float_literal( float c ){
	struct expr * e;

	e = expr_create(EXPR_FLOAT_LITERAL, NULL, NULL);
	e->literal_float_value = c;

	return e;
}


void expr_print( struct expr *e ){
    return;
}
