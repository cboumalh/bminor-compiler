
#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "utils.h"
#include "scope.h"
#include <stdio.h>
#include "scope_result.h"

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;

	int num_params;
    int num_locals;
    int num_total;
    int found_return;
};

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
void decl_print( struct decl *d, int indent );
void decl_resolve( struct decl *d, int verbose);
void decl_typecheck( struct decl *d );
void decl_codegen( struct decl *d, FILE *out );

#endif


