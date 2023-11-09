#include "param_list.h"
#include "resolve_result.h"


struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ){
	struct param_list * p = calloc(1, sizeof(struct param_list));

	memset(p, 0, sizeof(struct param_list));

    char *temp =  strdup(name);

	p->name = temp;
	p->type = type;
	p->next = next;

	return p;
}

void param_list_print( struct param_list *a ){
	while(a){
		printf("%s", a->name);
		printf(": ");
		type_print(a->type);

		if(a->next)
			printf(", ");

		a = a->next;
	}
}

void param_list_resolve(struct param_list *a, int verbose){
	while(a){
        a->symbol = symbol_create(SYMBOL_PARAM, a->type, a->name, 0);
		if(!scope_bind(a->name, a->symbol)){
			if(verbose)
            	printf("%s has already been declared in this scope!\n", a->name);
            resolve_result = 0;
        }
		a = a->next;
	}
}

struct param_list * param_copy(struct param_list *a){
	if(!a) return a;

	struct param_list * p = calloc(1, sizeof(struct param_list));

	memset(p, 0, sizeof(struct param_list));

	if(a->name)
		p->name = strdup(a->name);
	else
		p->name = NULL;

	p->type = type_copy(a->type);
	p->symbol = symbol_copy(a->symbol);
	p->next = param_copy(a->next);

	return p;
}

void param_delete(struct param_list *a){
	if(!a) return;

	free(a->name);
	type_delete(a->type);
	symbol_delete(a->symbol);

	param_delete(a->next);

	free(a);
}

int param_list_equals(struct param_list *a, struct param_list *b){
	if(!a && !b) return 1;
	if((a && !b) || (b && !a)) return 0;

	return type_equals(a->type, b->type) && param_list_equals(a->next, b->next);
}

void param_typecheck(struct type *t, struct param_list *p){
	while(p && t){
		if(!type_equals(p->type, t)){
			printf("type error: function parameter is of type ");
			type_print(p->type);
			printf(" but argument is of type ");
			type_print(t);
			printf("\n");
			typecheck_result = 0;
		}
		if(p->type->kind == TYPE_FUNCTION){
			typecheck_result = 0;
			printf("type error: (NOT SUPPORTED) functions cannot take other functions as arguments\n");
		}

		p = p->next;
		t = t->next;
	}

	if(p || t){
		printf("type error: number of arguments in function call is incorrect\n");
		typecheck_result = 0;
	}
}