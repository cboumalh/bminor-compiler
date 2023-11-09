#include "decl.h"
#include "resolve_result.h"
#include "typecheck_result.h"


struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ){
    struct decl * d = calloc(1, sizeof(struct decl));

	memset(d, 0, sizeof(struct decl));

    char *temp =  NULL;
    
    if(name)
        temp = strdup(name);

    d->name = temp;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;

    return d;
}


void decl_print( struct decl *d, int indent ){
    if(d->name){
        printf("%s: ", d->name);
        type_print(d->type);

        if(d->value){
            printf("= ");
            expr_print(d->value);
            printf(";");
        }
        else if(d->code){
            printf("= ");
            printf("{\n");
            
            struct stmt *code = d->code;
            while(code){
                stmt_print(code, indent);
                code = code->next;
            }
            printf("\n}");
        }
        else{
            printf(";");
        }

        printf("\n");
    }
}

void decl_resolve(struct decl *d){
    if(!d) return;

    if(d->name){
        expr_resolve(d->value);

        expr_resolve(d->type->size);

        symbol_t kind = scope_is_global() ? SYMBOL_GLOBAL : SYMBOL_LOCAL;

        d->symbol = symbol_create(kind, d->type, d->name, d->type->kind == TYPE_FUNCTION && d->code);
        

        if(!scope_bind(d->name, d->symbol)){
            printf("%s has already been declared in this scope!\n", d->name);
            resolve_result = 0;
        }
    
        if(d->type->params){
            scope_enter();
            param_list_resolve(d->type->params);
        }

        if(d->code) {
            scope_enter();
            stmt_resolve(d->code);
            scope_exit();
        }

        if(d->type->params) scope_exit();

    }

    decl_resolve(d->next);
}

void decl_typecheck( struct decl *d ){
    if(!d) return;
    if(d->name){
        if(d->value) {
            struct type *t;
            t = expr_typecheck(d->value);
            int type_eq_val = type_equals(t,d->symbol->type);
            if(!type_eq_val) {
                typecheck_result = 0;
                printf("type error: cannot assign ");
                expr_print(d->value);
                printf(" of type ");
                type_print(t);
                printf(" to %s\n", d->name);
            }

            if(type_eq_val && d->symbol->kind == SYMBOL_LOCAL && t && t->kind == TYPE_ARRAY){
                typecheck_result = 0;
                printf("type error: (NOT SUPPORTED) array initializer in the local scope\n");
            }

            if(type_eq_val && d->symbol->kind == SYMBOL_GLOBAL && is_var_expr(d->value)){
                typecheck_result = 0;
                printf("type error: (NOT SUPPORTED) non constant initializers in the global scope\n");
            }

        }

        if(d->symbol->type->kind == TYPE_ARRAY && !d->value && !d->symbol->type->size){
                typecheck_result = 0;
                printf("type error: cannot declare an array with no size without initializing it with a value\n");
        }

        if(d->code) {
            stmt_typecheck(d->code, d->symbol->type);
        }
    }


    decl_typecheck(d->next);
}