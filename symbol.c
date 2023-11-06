#include "symbol.h"

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name, int func_defined){
    struct symbol *s = malloc(sizeof(struct symbol));
    s->kind = kind;
    s->type = type;
    s->name = name;
    s->func_defined = func_defined;
    return s;
}

void symbol_print(struct symbol *sym){
    if(!sym) return;
    switch(sym->kind){
        case SYMBOL_GLOBAL:
            printf("global %s", sym->name);
            break;
        case SYMBOL_LOCAL:
            printf("local %d", sym->which);
            break;
        case SYMBOL_PARAM:
            printf("param %d", sym->which);
            break;
        default:
            puts(":/\n");
            break;
    }
}

void symbol_delete(struct symbol *sym){
    free(sym);
}