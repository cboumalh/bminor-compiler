#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef enum {
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_ASSIGN,
	EXPR_OR,
	EXPR_AND,
	EXPR_NOT_EQUAL,
	EXPR_MODULO,
	EXPR_EQ,
	EXPR_GREATER_OR_EQ,
	EXPR_GREATER,
	EXPR_LESS_OR_EQ,
	EXPR_LESS,
	EXPR_POWER,
	EXPR_UNARY,
	EXPR_PLUS,
	EXPR_NOT,
	EXPR_INCREMENT,
	EXPR_DECREMENT,
	EXPR_CALL,
	EXPR_SUBSCRIPT,
	EXPR_INTEGER_LITERAL,
	EXPR_STRING_LITERAL,
	EXPR_BOOL_LITERAL,
	EXPR_FLOAT_LITERAL,
	EXPR_NAME,
	EXPR_CHAR_LITERAL,
	EXPR_ARG,
	EXPR_ARRAY_DECL,
	EXPR_ONE_D_ARR

} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	float literal_float_value;
	const char * string_literal;
	struct symbol *symbol;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char *c );
struct expr * expr_create_string_literal( const char *str );
struct expr * expr_create_float_literal( float c );

void expr_print( struct expr *e );

#endif
