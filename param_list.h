
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "typecheck_result.h"

struct expr;

struct param_list {
	char *name;
	struct type *type;
	struct symbol *symbol;
	struct param_list *next;
};

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next );
void param_list_print( struct param_list *a );
void param_list_resolve(struct param_list *a);
struct param_list * param_copy(struct param_list *a);
void param_delete(struct param_list *a);
int param_list_equals(struct param_list *a, struct param_list *b);
void param_typecheck(struct type *, struct param_list *);

#endif
