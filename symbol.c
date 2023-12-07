#include "symbol.h"

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name, int func_defined){
    struct symbol *s = malloc(sizeof(struct symbol));
    s->kind = kind;
    s->type = type_copy(type);
    s->name = strdup(name);
    s->func_defined = func_defined;
    return s;
}

void symbol_print(struct symbol *sym){
    if(!sym) return;
    switch(sym->kind){
        case SYMBOL_GLOBAL:
            printf("global %s\n", sym->name);
            break;
        case SYMBOL_LOCAL:
            printf("local %d\n", sym->which);
            break;
        case SYMBOL_PARAM:
            printf("param %d\n", sym->which);
            break;
        default:
            puts(":/\n");
            break;
    }
}

void symbol_delete(struct symbol *sym){
    if(!sym) return;
    
    type_delete(sym->type);
    free(sym->name);
    free(sym);
}

struct symbol * symbol_copy(struct symbol *sym){
    if(!sym) return sym;

    struct symbol *cpy = malloc(sizeof(struct symbol));

    cpy->kind = sym->kind;
    cpy->which = sym->which;
    cpy->type = type_copy(sym->type);

    if(sym->name)
        cpy->name = strdup(sym->name);
    else
        cpy->name = NULL;

    cpy->func_defined = sym->func_defined;

    return cpy;
}

char *symbol_codegen(struct symbol *s) {
    char str[MAX_SYMBOL_SIZE];
    switch (s->kind) {
        case SYMBOL_GLOBAL:
            return s->name;
        case SYMBOL_PARAM:
            sprintf(str, "%d(%%rbp)", -8 * s->which);
            return strdup(str);
        case SYMBOL_LOCAL:
            sprintf(str, "%d(%%rbp)", -8 * s->which);
            return strdup(str);
        default:
            return 0;
    }
}