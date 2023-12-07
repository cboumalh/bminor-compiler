#include "scope.h"
#include "scope_result.h"


struct symbol *scope_bind(const char *name, struct symbol *sym){

    if(!hash_table_insert(sc->table, name, sym)){
        struct symbol *existing_sym = scope_lookup_top(name);

        if(existing_sym->type->kind != TYPE_FUNCTION || sym->type->kind != TYPE_FUNCTION)
            return NULL;
        
        if(!type_equals(existing_sym->type, sym->type))
            return NULL;

        if(!param_list_equals(existing_sym->type->params, sym->type->params))
            return NULL;

        if(type_equals(existing_sym->type, sym->type) && (existing_sym->func_defined && sym->func_defined))
            return NULL;

        if(type_equals(existing_sym->type, sym->type) && (existing_sym->func_defined == 1 || sym->func_defined == 1)){
            existing_sym->func_defined = 1;
            sym->func_defined = 1;
        }

        return existing_sym;
    }

    switch(sym->kind){
        case SYMBOL_GLOBAL:
            sym->which = 0;
            break;
        case SYMBOL_LOCAL:
            sym->which = ++sc->locals;
            break;
        case SYMBOL_PARAM:
            sym->which = ++sc->params;
            break;
        default:
            break;
    }

    return sym;
}

struct symbol *scope_lookup_top(const char *name){
    if(!sc) return NULL;

    return hash_table_lookup(sc->table, name);
}

struct symbol *scope_lookup(const char *name){
    if( !sc ) return NULL;

    struct scope *head = sc;
    struct symbol *result;

    while(head){
        result = hash_table_lookup(head->table, name);
        if(result) return result;
        head = head->next;
    }
    
    return NULL;
}

struct scope *scope_enter(){
    struct scope *head = scope_create();

    if(sc){
        head->next = sc;
        sc = head;
    }
    else
        sc = head;

    return sc;
}

struct scope *scope_exit(){
    struct scope *curr = sc;

    sc = sc->next;
    
    hash_table_delete(curr->table);
    free(curr);

    return sc;
}

struct scope *scope_create(){
    struct scope *head = malloc(sizeof(struct scope));
    if(!head){
        puts("[ERROR] Failed to allocate memory for struct scope. Exiting...");
        exit(EXIT_FAILURE);
    }

    head->table = hash_table_create(0,0);
    head->next = NULL;
    head->locals = 0;
    head->params = 0;

    return head;
}

bool scope_is_global(){
    return sc && sc->next == NULL;
}

int scope_level(){
    struct scope *head = sc;
    int count = 0;

    while(head){
        count++;
        head = head->next;
    }

    return count;

}
