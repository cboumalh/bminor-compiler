#include "decl.h"
#include "resolve_result.h"
#include "typecheck_result.h"

int First_global = 1;


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

    d->num_params = 0;
    d->num_locals = 0;
    d->num_total = 0;
    d->found_return = 0;

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

void decl_resolve(struct decl *d, int verbose){
    if(!d) return;

    if(d->name){
        expr_resolve(d->value, verbose);

        expr_resolve(d->type->size, verbose);

        symbol_t kind = scope_is_global() ? SYMBOL_GLOBAL : SYMBOL_LOCAL;

        d->symbol = symbol_create(kind, d->type, d->name, d->type->kind == TYPE_FUNCTION && d->code);

        if(!scope_bind(d->name, d->symbol)){
            if(verbose)
                printf("%s has already been declared in this scope!\n", d->name);
            resolve_result = 0;
        }
    
        if(d->type->params){
            scope_enter();
            param_list_resolve(d->type->params, verbose);
            d->num_params = sc->params;
        }

        if(d->code) {
            scope_enter();
            stmt_resolve(d->code, verbose);
            d->num_locals = sc->locals;
            d->num_total = d->num_params + d->num_locals;
            scope_exit();
        }

        if(d->type->params) scope_exit();

    }

    decl_resolve(d->next, verbose);
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
            if(d->symbol->type->subtype && (d->symbol->type->subtype->kind == TYPE_ARRAY || d->symbol->type->subtype->kind == TYPE_FUNCTION)){
                typecheck_result = 0;
                printf("type error: (NOT SUPPORTED) functions to return other functions/arrays\n");
		    }

            struct param_list * temp = d->symbol->type->params;
            while(temp){
                if(temp->type->kind == TYPE_FUNCTION){
			        typecheck_result = 0;
			        printf("type error: (NOT SUPPORTED) functions cannot take other functions as arguments\n");
                }
                temp = temp->next;
            }
            stmt_typecheck(d->code, d->symbol->type);
        }
    }


    decl_typecheck(d->next);
}

void decl_prologue(struct decl *d, FILE *out) {
    extern char *Arg_regs[6];
    extern bool Registers[7];

    fprintf(out, "# Start of function prologue\n");
    // setup framei
    fprintf(out, "\tpushq %%rbp\n");
    fprintf(out, "\tmovq %%rsp, %%rbp\n");

    // save params
    if(d->num_params > 6){
        printf("codegen error: more than 6 parameters not supported\n");
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < d->num_params; i++) {
        fprintf(out, "\tpushq %%%s\n", Arg_regs[i]);
    }

    // allocate locals
    if(d->num_locals) fprintf(out, "\tsubq $%d, %%rsp\n", (d->num_locals*8));

    // save registers
    if(strcmp(d->name, "man")) {
        fprintf(out, "\tpushq %%rbx\n");
        fprintf(out, "\tpushq %%r12\n");
        fprintf(out, "\tpushq %%r13\n");
        fprintf(out, "\tpushq %%r14\n");
        fprintf(out, "\tpushq %%r15\n");
    }
    fprintf(out, "# End of functiop prologue\n");
    
}

void decl_epilogue(struct decl *d, FILE *out) {
    
    fprintf(out, "# Start of function epilogue\n");
    // restore registers
    fprintf(out, ".%s_epilogue:\n", d->name);
    
    if(strcmp(d->name, "man")) {
        fprintf(out, "\tpopq %%r15\n");
        fprintf(out, "\tpopq %%r14\n");
        fprintf(out, "\tpopq %%r13\n");
        fprintf(out, "\tpopq %%r12\n");
        fprintf(out, "\tpopq %%rbx\n");
    }

    // deallocate locals

    // restore frame
    fprintf(out, "\tmovq %%rbp, %%rsp\n");
    fprintf(out, "\tpopq %%rbp\n");
    fprintf(out, "\tret\n");
    fprintf(out, "# End of function epilogue\n");
}

void decl_codegen_array(struct decl *d, FILE *out, int g) {
    if(d->type->subtype->kind == TYPE_ARRAY){
        printf("codegen error: multidimensional array not supported\n");
        exit(EXIT_FAILURE);
    }

    if(g) fprintf(out, "%s:\n", d->name);
    struct expr *e = 0;
    if(d->value){
        e = d->value;
        e = e->left;
    }
    if(d->type->size){
        for (int i = 0; i < d->type->size->literal_value; i++) {
            if(e){
                fprintf(out, "\t.quad %d\n", e->left->literal_value);
                e = e->right;
            }
            else{
                fprintf(out, "\t.quad %d\n", 0);
            }
        }
    }
    else{ 
        while(e){
            fprintf(out, "\t.quad %d\n", e->left->literal_value);
            e = e->right;
        }
    }
}

void decl_codegen(struct decl *d, FILE *out) {
    if(!d) return;
    
    
    if(d->name){
        if(d->symbol->kind == SYMBOL_GLOBAL) {
            
            switch (d->symbol->type->kind) {
                case TYPE_FUNCTION: 
                    if(d->code) {
                        // first to make declaring a bunch in a row
                        First_global = 1;
                        fprintf(out, ".text\n");
                        fprintf(out, ".global %s\n", d->name);
                        fprintf(out, "%s:\n", d->name);
                        decl_prologue(d, out);
                        stmt_codegen(d->code, d, out);
                        decl_epilogue(d, out);
                    }
                    break;
            
                case TYPE_INTEGER:
                case TYPE_BOOLEAN:
                    if(First_global) {
                        fprintf(out, ".data\n");
                        First_global = 0;
                    }
                    fprintf(out, ".global %s\n", d->name);
                    fprintf(out, "%s:\n\t.quad %d\n", d->name, d->value ? d->value->literal_value : 0);
                    break;
                case TYPE_STRING:
                    if(First_global) {
                        fprintf(out, ".data\n");
                        First_global = 0;
                    }
                    fprintf(out, ".global %s\n", d->name);
                    if(d->value) {
                        int l = scratch_label_create(LABEL_STRING);
                        fprintf(out, "%s:\n\t.quad %s\n", d->name, scratch_label_name(l, LABEL_STRING));
                        fprintf(out, "%s:\n\t.string %s\n", scratch_label_name(l, LABEL_STRING), d->value->string_literal);
                    }
                    break;
                case TYPE_CHARACTER: 
                    if(First_global) {
                        fprintf(out, ".data\n");
                        First_global = 0;
                    }
                    fprintf(out, ".global %s\n", d->name);
                    fprintf(out, "%s:\n\t.quad %d\n", d->name, d->value ? *(d->value->string_literal+1) : 'a');
                    break;
                case TYPE_ARRAY:
                    if(First_global) {
                        fprintf(out, ".data\n");
                        First_global = 0;
                    }
                    fprintf(out, ".global %s\n", d->name);
                    decl_codegen_array(d, out, 1);
                    break;
                default:
                    break;
            }
        

        // Local decls
        } else {
            int l1, r1;
            switch (d->symbol->type->kind) {
                case TYPE_CHARACTER:
                case TYPE_STRING:
                case TYPE_BOOLEAN:
                case TYPE_INTEGER:
                    if(d->value) {
                        fprintf(out, "# generating code for value of decl %s\n", d->name);
                        expr_codegen(d->value, out);
                        fprintf(out, "# generating code for decl %s\n", d->name);
                        fprintf(out, "\tmovq %%%s, %s\n", 
                                scratch_reg_name(d->value->reg), 
                                symbol_codegen(d->symbol));
                        scratch_reg_free(d->value->reg);

                    }
                    break; 
                default:
                    break;
            }
        }
    }

    decl_codegen(d->next, out);
}