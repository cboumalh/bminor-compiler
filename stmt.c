#include "stmt.h"
#include "scope_result.h"
#include "typecheck_result.h"


struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ){
    struct stmt * s = calloc(1, sizeof(struct stmt));

	memset(s, 0, sizeof(struct stmt));

    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;

    return s;
}

void stmt_print( struct stmt *s, int indent ){
    if(s){
        if(s->kind == STMT_DECL){
            printf("\n");
            indent_print(indent);

            decl_print(s->decl, indent);
        }

        else if(s->kind == STMT_EXPR){
            printf("\n");
            indent_print(indent);

            expr_print(s->expr);

            printf(";");
        }

        else if(s->kind == STMT_IF_ELSE){
            printf("\n");
            indent_print(indent);

            printf("if(");
            expr_print(s->expr);
            printf(")");

            stmt_print(s->body, indent + INDENT);

            printf("\n");
            indent_print(indent);
            printf("else");

            stmt_print(s->else_body, indent + INDENT);
        }

        else if(s->kind == STMT_FOR){
            printf("\n");
            indent_print(indent);

            printf("for(");
            expr_print(s->init_expr);
            printf(";");
            expr_print(s->expr);
            printf(";");
            expr_print(s->next_expr);
            printf(")");

            stmt_print(s->body, indent + INDENT);
        }

        else if(s->kind == STMT_PRINT){
            printf("\n");
            indent_print(indent);

            printf("print ");
            struct expr *expr_list = s->expr;
            expr_print(expr_list);
            printf(";");

        }

        else if(s->kind == STMT_RETURN){
            printf("\n");
            indent_print(indent);

            printf("return ");

            expr_print(s->expr);

            printf(";");

        }

        else if(s->kind == STMT_BLOCK){
            printf("{");
            stmt_print(s->body, indent);

            s = s->body->next;

            while(s){
                stmt_print(s, indent);
                s = s->next;
            }

            printf("\n");
            indent_print(indent - INDENT);
            printf("}");
        }

        else if(s->kind == STMT_WHILE){
            printf("\n");
            indent_print(indent);

            printf("while(");
            expr_print(s->expr);
            printf(")");


            stmt_print(s->body, indent + INDENT);
        }

        else if(s->kind == STMT_IF){
            printf("\n");
            indent_print(indent);

            printf("if(");
            expr_print(s->expr);
            printf(")");

            stmt_print(s->body, indent + INDENT);
        }

        else if(s->kind == STMT_SEMICOLON){
            printf(";");
        }
    }
}

void stmt_resolve(struct stmt *s, int verbose){
    if(!s) return;

    if(s->kind != STMT_COMMENT){
        decl_resolve(s->decl, verbose);

        expr_resolve(s->init_expr, verbose);
        expr_resolve(s->expr, verbose);
        expr_resolve(s->next_expr, verbose);

        if(s->kind == STMT_BLOCK){
            sc = scope_enter();
            stmt_resolve(s->body, verbose);
            sc = scope_exit();

        }
        else{
            stmt_resolve(s->body, verbose);
            stmt_resolve(s->else_body, verbose);
        }
    }

    stmt_resolve(s->next, verbose);

}

void stmt_typecheck(struct stmt *s, struct type *decl_type){
    if(!s) return;

    struct type *t;
    
    if(s->kind == STMT_EXPR){
        t = expr_typecheck(s->expr);
        type_delete(t);
    }
    else if(s->kind == STMT_IF_ELSE){
        t = expr_typecheck(s->expr);
        if(t->kind!=TYPE_BOOLEAN) {
            typecheck_result = 0;
            printf("type error: expression for if else must be a boolean but ");
            expr_print(s->expr);
            printf(" is of type ");
            type_print(t);
            printf("\n");
        }
        type_delete(t);
        stmt_typecheck(s->body, decl_type);
        stmt_typecheck(s->else_body, decl_type);
    }
    else if(s->kind == STMT_IF){
        t = expr_typecheck(s->expr);
        if(t->kind!=TYPE_BOOLEAN) {
            typecheck_result = 0;
            printf("type error: expression for if must be a boolean but ");
            expr_print(s->expr);
            printf(" is of type ");
            type_print(t);
            printf("\n");
        }
        type_delete(t);
        stmt_typecheck(s->body, decl_type);
    }
    else if(s->kind == STMT_WHILE){
        t = expr_typecheck(s->expr);
        if(t->kind!=TYPE_BOOLEAN) {
            typecheck_result = 0;
            printf("type error: expression for while must be a boolean but ");
            expr_print(s->expr);
            printf(" is of type ");
            type_print(t);
            printf("\n");
        }
        type_delete(t);
        stmt_typecheck(s->body, decl_type);
    }
    else if(s->kind == STMT_DECL){
        decl_typecheck(s->decl);
    }
    else if(s->kind == STMT_BLOCK){
        stmt_typecheck(s->body, decl_type);
    }
    else if(s->kind == STMT_FOR){
        t = expr_typecheck(s->expr);
        
        if(t && t->kind != TYPE_BOOLEAN){
            typecheck_result = 0;
            printf("type error: second expr for for loop must be a boolean but ");
            expr_print(s->expr);
            printf(" is of type ");
            type_print(t);
            printf("\n");
        }

        stmt_typecheck(s->body, decl_type);
    }
    else if(s->kind == STMT_RETURN){
        t = expr_typecheck(s->expr);

        if(t && !type_equals(decl_type->subtype, t)){
            typecheck_result = 0;
            printf("type error: function return type ");
            type_print(decl_type->subtype);
            printf("does not match type returned ");
            type_print(t);
            printf("\n");
        }
        else if(!t && decl_type->subtype->kind != TYPE_VOID){
            typecheck_result = 0;
            printf("type error: function must return a ");
            type_print(decl_type->subtype);
            printf("\n");
        }
    }
    else if(s->kind == STMT_PRINT){
        t = expr_typecheck(s->expr);

        while(t){
            if(t->kind == TYPE_ARRAY || t->kind == TYPE_FUNCTION || t->kind == TYPE_VOID){
                typecheck_result = 0;
                printf("type error: you are trying to print a ");
                type_print(t);
                printf(" which is not allowed \n");
            }
            t = t->next;
        }

    }

    stmt_typecheck(s->next, decl_type);
}

void stmt_print_codegen(struct stmt *s, struct decl *d, FILE *out) {
    struct expr *curr = s->expr;
    struct type *t;
    fprintf(out, "# printing exprs\n");
    while(curr) {
        t = expr_typecheck(curr);
        fprintf(out, "\tpushq %%r10\n");
        fprintf(out, "\tpushq %%r11\n");
        expr_codegen(curr, out);
        fprintf(out, "\tmovq %%%s, %%rdi\n", scratch_reg_name(curr->reg));
        switch (t->kind) {
            case TYPE_INTEGER:                
                fprintf(out, "\tcall print_integer\n");
                break;
            case TYPE_STRING:
                fprintf(out, "\tcall print_string\n");
                break;
            case TYPE_CHARACTER:
                fprintf(out, "\tcall print_character\n");
                break;
            case TYPE_BOOLEAN:
                fprintf(out, "\tcall print_boolean\n");
                break;
        }
        fprintf(out, "\tpopq %%r11\n");
        fprintf(out, "\tpopq %%r10\n");

        scratch_reg_free(curr->left->reg);
        curr = curr->right;
    }
}

void stmt_while_codegen(struct stmt *s, struct decl *d, FILE *out) {
    fprintf(out, "#generating code for while loop\n");

    // top and done labels
    int top = scratch_label_create(LABEL_JUMP);
    int done = scratch_label_create(LABEL_JUMP);
    fprintf(out, "%s:\n", scratch_label_name(top, LABEL_JUMP));

    // expr
    fprintf(out, "# condition expr\n");
    expr_codegen(s->expr, out);
    fprintf(out, "\tcmpq $0, %%%s\n", scratch_reg_name(s->expr->reg));
    fprintf(out, "\tje %s\n", scratch_label_name(done, LABEL_JUMP));
    
    // body
    fprintf(out, "# while body\n");
    stmt_codegen(s->body, d, out);

    // jmp top
    fprintf(out, "\tjmp %s\n", scratch_label_name(top, LABEL_JUMP));

    // done label
    fprintf(out, "%s:\n", scratch_label_name(done, LABEL_JUMP));
    
    scratch_reg_free(s->expr->reg);
}

void stmt_for_codegen(struct stmt *s, struct decl *d, FILE *out) {
    fprintf(out, "# generating code for loop\n");

    // init expr
    fprintf(out, "# init expr\n");
    expr_codegen(s->init_expr, out);


    // top and done label
    int top = scratch_label_create(LABEL_JUMP);
    int done = scratch_label_create(LABEL_JUMP);
    fprintf(out, "%s:\n", scratch_label_name(top, LABEL_JUMP));

    // expr
    fprintf(out, "# condition expr\n");
    expr_codegen(s->expr, out);
    if(s->expr) fprintf(out, "\tcmpq $0, %%%s\n", scratch_reg_name(s->expr->reg));
    if(s->expr) fprintf(out, "\tje %s\n", scratch_label_name(done, LABEL_JUMP));
    

    // body
    fprintf(out, "# for body\n");
    stmt_codegen(s->body, d, out);

    // next expr
    fprintf(out, "# next expr\n");
    expr_codegen(s->next_expr, out);

    // jmp top
    fprintf(out, "\tjmp %s\n", scratch_label_name(top, LABEL_JUMP));

    // done label
    fprintf(out, "%s:\n", scratch_label_name(done, LABEL_JUMP));

    if(s->init_expr) scratch_reg_free(s->init_expr->reg);
    if(s->expr) scratch_reg_free(s->expr->reg);
    if(s->next_expr) scratch_reg_free(s->next_expr->reg);
}    

void stmt_if_codegen(struct stmt *s, struct decl *d, FILE *out) {
    fprintf(out, "# generating code for if stmt\n");

    // l1: false; l2: done
    int l1 = scratch_label_create(LABEL_JUMP);
    int l2 = scratch_label_create(LABEL_JUMP);;
    
    // expr cond
    fprintf(out, "# condition expr\n");
    expr_codegen(s->expr, out);

    
    fprintf(out, "\tcmpq $0, %%%s\n", scratch_reg_name(s->expr->reg));
    fprintf(out, "\tje %s\n", scratch_label_name(l1, LABEL_JUMP));
    
    // if body
    stmt_codegen(s->body, d, out);
    fprintf(out, "\tjmp %s\n", scratch_label_name(l2, LABEL_JUMP));

    // false label
    fprintf(out, "%s:\n", scratch_label_name(l1, LABEL_JUMP));

    // else body
    stmt_codegen(s->else_body, d, out);

    // done label
    fprintf(out, "%s:\n", scratch_label_name(l2, LABEL_JUMP));

    scratch_reg_free(s->expr->reg);
}


void stmt_codegen(struct stmt *s, struct decl *d, FILE *out) {
    if(!s) return;
    
     switch(s->kind) {
        case STMT_BLOCK:
            stmt_codegen(s->body, d, out);
            break;
        case STMT_EXPR:
            s->expr->func_value = 1;
            expr_codegen(s->expr, out);
            scratch_reg_free(s->expr->reg);
            break;    
        case STMT_RETURN:
            fprintf(out, "# generating code for return\n");
            expr_codegen(s->expr, out);
            fprintf(out, "# generating code for return\n");
            if(s->expr) fprintf(out, "\tmovq %%%s, %%rax\n", scratch_reg_name(s->expr->reg));
            fprintf(out, "\tjmp .%s_epilogue\n", d->name);
            if(s->expr) scratch_reg_free(s->expr->reg);
            break;
        case STMT_DECL:
            decl_codegen(s->decl, out);              
            break;
        case STMT_IF_ELSE:
            stmt_if_codegen(s, d, out);
            break;
        case STMT_WHILE:
            stmt_while_codegen(s, d, out);
            break;
        case STMT_PRINT:
            stmt_print_codegen(s, d, out);
            break;
        case STMT_FOR:
            stmt_for_codegen(s, d, out);
            break;
        default:
            break;
    }

    stmt_codegen(s->next, d, out);
}