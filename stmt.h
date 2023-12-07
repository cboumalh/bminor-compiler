
#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include "utils.h"
#include "scratch.h"

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK,
	STMT_WHILE,
	STMT_IF,
	STMT_COMMENT,
	STMT_SEMICOLON
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
};


struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next );
void stmt_print( struct stmt *s, int indent );
void stmt_resolve(struct stmt *s, int verbose);
void stmt_typecheck( struct stmt *s, struct type * decl_type);
void stmt_codegen( struct stmt *s, struct decl *d, FILE *out );



#endif
