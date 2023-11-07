#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type *type;
	char *name;
	int which;
	int func_defined;
};

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name, int func_defined);

void symbol_print(struct symbol *sym);

struct symbol * symbol_copy(struct symbol *sym);

void symbol_delete(struct symbol *sym);

#endif
