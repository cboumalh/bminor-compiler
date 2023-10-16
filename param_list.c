#include "param_list.h"


struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ){
	struct param_list * p = calloc(1, sizeof(struct param_list));

	memset(p, 0, sizeof(struct param_list));

    char *temp =  strdup(name);

	int i = 0;
	while(temp[i] != '\0' && temp[i] != ':')
		i++;

	temp[i] = '\0';

	p->name = temp;
	p->type = type;
	p->next = next;

	return p;
}

void param_list_print( struct param_list *a ){
    return;
}