#include "expr.h"
#include "resolve_result.h"
#include "typecheck_result.h"


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

			if(e->left)
				printf("[");

			expr_print(e->right);

			if(e->left)
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
			printf("}");
			if(e->right)
				printf(",");
			expr_print(e->right);
		}
		else if(e->kind == EXPR_ONE_D_ARR){
			printf("{");
			expr_print(e->left);
			printf("}");
			if(e->right)
				printf(",");
			expr_print(e->right);
		}
		else if(e->kind == EXPR_PARAN){
			printf("(");
			expr_print(e->right);
			printf(")");
		}

	}
}


void expr_resolve(struct expr *e, int verbose){
	if(!e) return;

	if(e->kind == EXPR_NAME) {
		e->symbol = scope_lookup(e->name);
		if (!e->symbol){
			if(verbose)
            	printf("[ERROR|resolve] Variable %s used before declaration\n", e->name);
			resolve_result = 0;
        }
        else{
			if(verbose){
            	printf("Variable %s resolved to ", e->name);
            	symbol_print(e->symbol);
				printf("\n");
			}
        }
	} 
	else {
		expr_resolve(e->left, verbose);
		expr_resolve(e->right, verbose);
	}
}

struct type * expr_typecheck( struct expr *e ){
	if(!e) return NULL;

	struct type *lt = expr_typecheck(e->left);
	struct type *rt = expr_typecheck(e->right);

	struct type *result;

	if (e->kind == EXPR_INTEGER_LITERAL) {
		result = type_create(TYPE_INTEGER, 0, 0, 0, 0);
	} 
	else if (e->kind == EXPR_STRING_LITERAL) {
		result = type_create(TYPE_STRING, 0, 0, 0, 0);
	} 
	else if (e->kind == EXPR_CHAR_LITERAL) {
		result = type_create(TYPE_CHARACTER, 0, 0, 0, 0);
	} 
	else if (e->kind == EXPR_BOOL_LITERAL) {
		result = type_create(TYPE_BOOLEAN, 0, 0, 0, 0);
	} 
	else if (e->kind == EXPR_FLOAT_LITERAL) {
		result = type_create(TYPE_FLOAT, 0, 0, 0, 0);
	} 
	else if (e->kind == EXPR_NAME) {
		result = type_copy(e->symbol->type);
	} 
	else if (e->kind == EXPR_CALL) {
		if (lt->kind != TYPE_FUNCTION) {
			typecheck_result = 0;
			printf("type error: function name has to be of type function: ");
			expr_print(e->left);
			printf(" is of type ");
			type_print(lt);
		}
		param_typecheck(rt, lt->params);
		result = type_copy(lt->subtype);
	} 
	else if (e->kind == EXPR_ONE_D_ARR) {
		struct type *temp = lt;
		while (temp && temp->next) {
			if (!type_equals(temp, temp->next)){
				printf("type error: array has two elements of different kinds ");
				type_print(temp);
				printf("and ");
				type_print(temp->next);
				printf("\n");
				typecheck_result = 0;
			}
			if(temp->kind == TYPE_FUNCTION || temp->next->kind == TYPE_FUNCTION){
				typecheck_result = 0;
				printf("type error: (NOT SUPPORTED) array element cannot be a function\n");
			}
			temp = temp->next;
		}
		result = type_create(TYPE_ARRAY, type_create(lt->kind, 0, 0, 0, 0), 0, 0, 0);
	} 
	else if (e->kind == EXPR_PARAN) {
		result = type_copy(rt);
	} 
	else if (e->kind == EXPR_ARRAY_DECL) {
		result = type_create(TYPE_ARRAY, lt, 0, 0, 0);
	} 
	else if (e->kind == EXPR_ARG) {
		result = type_copy(lt);
		result->next = type_copy(rt);
	} 
	else if (e->kind == EXPR_SUBSCRIPT) {
		if(e->left){
			if (lt->kind == TYPE_ARRAY) {
				if (rt->kind != TYPE_INTEGER) {
					typecheck_result = 0;
					printf("type error: array index ");
					expr_print(e->right);
					printf(" of type ");
					type_print(lt);
					printf("is not an integer\n");
				}
				result = type_copy(lt->subtype);
			} else {
				typecheck_result = 0;
				printf("type error: variable does not match array: ");
				expr_print(e->left);
				printf("\n");
				result = type_copy(lt);
			}
		}
		else{
			result = type_copy(rt);
		}
	} 
	else if (e->kind == EXPR_PLUS) {
		if(rt->kind != TYPE_INTEGER && rt->kind != TYPE_FLOAT){
			typecheck_result = 0;
			printf("type error: ");
			expr_print(e->right);
			printf(" is a ");
			type_print(rt);
			printf(", only floats/integers are acceptable\n");
		}
		result = type_copy(rt);
	} 
	else if (e->kind == EXPR_DECREMENT) {
		if(lt){
			if(e->left->kind != EXPR_NAME && e->left->kind != EXPR_SUBSCRIPT){
				printf("type error: there is an attempt to decrement an expression that is not a variable: ");
				expr_print(e->left);
				printf("\n");
				typecheck_result = 0;
			}
			if(lt->kind != TYPE_INTEGER){
				printf("type error: ");
				expr_print(e->left);
				printf(" is of type ");
				type_print(lt);
				printf("and we can only decrement integers\n");
				typecheck_result = 0;
			}
		}
		else if(rt){
			if(e->right->kind != EXPR_NAME && e->right->kind != EXPR_SUBSCRIPT){
				printf("type error: there is an attempt to decrement an expression that is not a variable: ");
				expr_print(e->right);
				printf("\n");
				typecheck_result = 0;
			}
			if(rt->kind != TYPE_INTEGER){
				typecheck_result = 0;
				printf("type error: ");
				expr_print(e->right);
				printf(" is of type ");
				type_print(rt);
				printf("and we can only decrement integers\n");
			}
		}
		result = lt ? type_copy(lt) : type_copy(rt);
	} 
	else if (e->kind == EXPR_INCREMENT) {
		if(lt){
			if(e->left->kind != EXPR_NAME && e->left->kind != EXPR_SUBSCRIPT){
				printf("type error: there is an attempt to increment an expression that is not a variable: ");
				expr_print(e->left);
				printf("\n");
				typecheck_result = 0;
			}
			if(lt->kind != TYPE_INTEGER){
				printf("type error: ");
				expr_print(e->left);
				printf(" is of type ");
				type_print(lt);
				printf("and we can only increment integers\n");
				typecheck_result = 0;
			}
		}
		else if(rt){
			if(e->right->kind != EXPR_NAME && e->right->kind != EXPR_SUBSCRIPT){
				printf("type error: there is an attempt to increment an expression that is not a variable: ");
				expr_print(e->right);
				printf("\n");
				typecheck_result = 0;
			}
			if(rt->kind != TYPE_INTEGER){
				typecheck_result = 0;
				printf("type error: ");
				expr_print(e->right);
				printf(" is of type ");
				type_print(rt);
				printf("and we can only increment integers\n");
			}
		}
		result = lt ? type_copy(lt) : type_copy(rt);
	} 
	else if (e->kind == EXPR_NOT) {
		if(rt->kind != TYPE_BOOLEAN && rt->kind != TYPE_INTEGER){
			typecheck_result = 0;
			printf("type error: ");
			expr_print(e->right);
			printf(" is a ");
			type_print(rt);
			printf(", only booleans/integers are acceptable\n");
		}
		result = type_copy(rt);
	} 
	else if (e->kind == EXPR_UNARY) {
		if(rt->kind != TYPE_INTEGER && rt->kind != TYPE_FLOAT){
			typecheck_result = 0;
			printf("type error: ");
			expr_print(e->right);
			printf(" is a ");
			type_print(rt);
			printf(", only floats/integers are acceptable\n");
		}
		result = type_copy(rt);
	}
	else if (e->kind == EXPR_POWER || e->kind == EXPR_ADD || e->kind == EXPR_DIV || e->kind == EXPR_MUL || e->kind == EXPR_SUB) {
		if(rt->kind == TYPE_FLOAT && (lt->kind == TYPE_INTEGER || lt->kind == TYPE_FLOAT))
			result = type_create(TYPE_FLOAT, 0, 0, 0, 0);
		else if(rt->kind == TYPE_INTEGER && lt->kind == TYPE_INTEGER)
			result = type_create(TYPE_INTEGER, 0, 0, 0, 0);
		else if (rt->kind == TYPE_INTEGER && lt->kind == TYPE_FLOAT)
			result = type_create(TYPE_FLOAT, 0, 0, 0, 0);
		else{
			
			typecheck_result = 0;
			result = type_create(TYPE_INTEGER, 0, 0, 0, 0);
		}
	}
	else if(e->kind == EXPR_MODULO){
		if(rt->kind != TYPE_INTEGER || lt->kind != TYPE_INTEGER){
			printf("Cannot perform modulo on non ints\n");
			typecheck_result = 0;
		}

		result = type_create(TYPE_INTEGER, 0, 0, 0, 0);
	}
	else if(e->kind == EXPR_LESS || e->kind == EXPR_GREATER || e->kind == EXPR_GREATER_OR_EQ || e->kind == EXPR_LESS_OR_EQ){
		if(!type_equals(lt,rt)) {
			printf("type error: cannot compare ");
			expr_print(e->left);
			printf(" of type ");
			type_print(lt);
			printf("with ");
			expr_print(e->right);
			printf(" of type ");
			type_print(rt);
			printf("\n");
			typecheck_result = 0;
		}

		if(lt->kind==TYPE_VOID ||
		lt->kind==TYPE_ARRAY ||
		lt->kind==TYPE_FUNCTION ||
		lt->kind==TYPE_STRING ||
		lt->kind == TYPE_BOOLEAN) {
			typecheck_result = 0;
			printf("type error: cannot compare objects of type ");
			type_print(rt);
			printf("\n");
		}
		result = type_create(TYPE_BOOLEAN, 0, 0, 0, 0);
	}
	else if(e->kind == EXPR_EQ || e->kind == EXPR_NOT_EQUAL){
		if(!type_equals(lt,rt)) {
			printf("type error: cannot compare ");
			expr_print(e->left);
			printf(" of type ");
			type_print(lt);
			printf("with ");
			expr_print(e->right);
			printf(" of type ");
			type_print(rt);
			printf("\n");
			typecheck_result = 0;
		}

		if(lt->kind==TYPE_VOID ||
		lt->kind==TYPE_ARRAY ||
		lt->kind==TYPE_FUNCTION) {
            typecheck_result = 0;
			printf("type error: cannot compare objects of type ");
			type_print(rt);
			printf("\n");
		}
		result = type_create(TYPE_BOOLEAN, 0, 0, 0, 0);
	}
	else if(e->kind == EXPR_AND || e->kind == EXPR_OR){
		if(lt->kind != TYPE_BOOLEAN || rt->kind != TYPE_BOOLEAN){
            typecheck_result = 0;
			printf("type error: cannot use logical operators on ");
			expr_print(e->left);
			printf(" of type ");
			type_print(lt);
			printf("with ");
			expr_print(e->right);
			printf(" of type ");
			type_print(rt);
			printf("booleans only are excepted\n");
		}

		result = type_create(TYPE_BOOLEAN, 0, 0, 0, 0);
	}
	else if(e->kind == EXPR_ASSIGN){
		if(!type_equals(lt,rt)) {
			printf("type error: cannot assign ");
			expr_print(e->left);
			printf(" of type ");
			type_print(lt);
			printf("to ");
			expr_print(e->right);
			printf(" of type ");
			type_print(rt);
			printf("\n");
			typecheck_result = 0;
		}

		if(e->left->kind != EXPR_NAME && e->left->kind != EXPR_SUBSCRIPT) {
			printf("type error: we can only assign to variables: ");
			expr_print(e->left);
			printf(" of type ");
			type_print(lt);
			printf("is not a variable\n");
            typecheck_result = 0;
		}

		
		result = type_copy(rt);
	}


	type_delete(lt);
	type_delete(rt);

	return result;
}

struct expr * expr_copy(struct expr *e){
    if(!e) return e;

    struct expr * cpy = calloc(1, sizeof(struct expr));

	memset(cpy, 0, sizeof(struct expr));

	cpy->kind = e->kind;

	if(e->name)
		cpy->name = strdup(e->name);
	else
		cpy->name = NULL;

	cpy->literal_value = e->literal_value;
	cpy->literal_float_value = e->literal_float_value;

	if(e->string_literal)
		cpy->string_literal = strdup(e->string_literal);
	else
		cpy->name = NULL;

	cpy->symbol = symbol_copy(e->symbol);

	cpy->left = expr_copy(e->left);
	cpy->right = expr_copy(e->right);

	return cpy;
}

void expr_delete(struct expr *e){
	if(!e) return;

	expr_delete(e->left);
	expr_delete(e->right);

	if(e->name){
		char* nonConstPointer = (char*)e->name;
    	free(nonConstPointer);
	}
	if(e->string_literal){
		char* nonConstPointer = (char*)e->string_literal;
    	free(nonConstPointer);
	}

	symbol_delete(e->symbol);
	
	free(e);
}