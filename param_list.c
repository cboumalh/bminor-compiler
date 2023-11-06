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

void param_list_resolve(struct param_list *a){
	while(a){
        a->symbol = symbol_create(SYMBOL_PARAM, a->type, a->name, 0);
		if(!scope_bind(a->name, a->symbol)){
            printf("%s has already been declared in this scope!\n", a->name);
            resolve_result = 0;
        }
		a = a->next;
	}
}