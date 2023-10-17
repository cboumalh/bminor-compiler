#include "decl.h"

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ){
    struct decl * d = calloc(1, sizeof(struct decl));

	memset(d, 0, sizeof(struct decl));

    char *temp =  strdup(name);

	int i = 0;
	while(temp[i] != '\0' && temp[i] != ':')
		i++;

	temp[i] = '\0';

    d->name = temp;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;

    return d;
}