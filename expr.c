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
	e->string_literal = strdup(c);

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
    if(e){

		if(e->kind == EXPR_CHAR_LITERAL){
			printf("%s", e->string_literal);
		}

		else if(e->kind == EXPR_STRING_LITERAL){
			printf("%s", e->string_literal);
		}

		else if(e->kind == EXPR_FLOAT_LITERAL){
			printf("%f", e->literal_float_value);
		}

		else if(e->kind == EXPR_BOOL_LITERAL){
			printf("%s",e->literal_value?"true":"false");
		}

		else if(e->kind == EXPR_INTEGER_LITERAL){
			printf("%d", e->literal_value);
		}

		else if(e->kind == EXPR_NAME){
			printf("%s", e->name);
		}

		else if(e->kind == EXPR_ADD){
			expr_print(e->left);
			printf("+");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_MUL){
			expr_print(e->left);
			printf("*");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_DIV){
			expr_print(e->left);
			printf("/");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_SUB){
			expr_print(e->left);
			printf("-");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_OR){
			expr_print(e->left);
			printf("||");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_AND){
			expr_print(e->left);
			printf("&&");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_MODULO){
			expr_print(e->left);
			printf("%%");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_POWER){
			expr_print(e->left);
			printf("^");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_GREATER){
			expr_print(e->left);
			printf(">");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_GREATER_OR_EQ){
			expr_print(e->left);
			printf(">=");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_EQ){
			expr_print(e->left);
			printf("==");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_NOT_EQUAL){
			expr_print(e->left);
			printf("!=");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_LESS_OR_EQ){
			expr_print(e->left);
			printf("<=");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_LESS){
			expr_print(e->left);
			printf("<");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_ASSIGN){
			expr_print(e->left);
			printf("=");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_UNARY){
			expr_print(e->left);
			printf("-");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_PLUS){
			expr_print(e->left);
			printf("+");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_NOT){
			expr_print(e->left);
			printf("!");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_INCREMENT){
			expr_print(e->left);
			printf("++");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_DECREMENT){
			expr_print(e->left);
			printf("--");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_CALL){
			expr_print(e->left);

			printf("(");

			expr_print(e->right);

			printf(")");
		}

		else if(e->kind == EXPR_SUBSCRIPT){
			expr_print(e->left);
			printf("[");
			expr_print(e->right);
			printf("]");
		}

		else if(e->kind == EXPR_ARG){
			expr_print(e->left);
			if(e->right)
				printf(",");
			expr_print(e->right);
		}

		else if(e->kind == EXPR_ARRAY_DECL){
			printf("{");
			expr_print(e->left);
			expr_print(e->right);
			printf("}");
		}

	}
}
