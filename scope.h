#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"
#include "hash_table.h"
#include <stdbool.h>

struct scope {
    struct  hash_table *table;
    struct  scope *next;
    int     locals;
    int     params;
};

struct symbol *scope_bind(const char *name, struct symbol *sym);
struct symbol *scope_lookup(const char *name);
struct symbol *scope_lookup_top(const char *name);

struct scope *scope_enter();
struct scope *scope_exit();
bool scope_is_global();
int scope_level();
struct scope *scope_create();

#endif