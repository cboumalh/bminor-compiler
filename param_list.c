#include "param_list.h"


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