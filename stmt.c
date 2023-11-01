#include "stmt.h"


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