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

void stmt_resolve(struct stmt *s){
    if(!s) return;

    if(s->kind != STMT_COMMENT){
        decl_resolve(s->decl);

        expr_resolve(s->init_expr);
        expr_resolve(s->expr);
        expr_resolve(s->next_expr);

        sc = s->kind == STMT_BLOCK ? scope_enter() : sc;
        stmt_resolve(s->body);
        sc = s->kind == STMT_BLOCK ? scope_exit(sc) : sc;
    }

    stmt_resolve(s->next);

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
            printf(" does not match type returned ");
            type_print(t);
            printf("\n");
        }
        else if(!t && decl_type->kind != TYPE_VOID){
            typecheck_result = 0;
            printf("type error: function must return a ");
            type_print(t);
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