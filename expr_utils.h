#ifndef EXPR_UTILS_H
#define EXPR_UTILS_H
#include "expr.h"

struct Node {
    struct expr * data;
    struct Node* next;
};

struct Stack {
    struct Node* top;
};


void initializeStack(struct Stack* stack);
int isEmpty(struct Stack* stack);
void push(struct Stack* stack, struct expr *value);
struct expr * pop(struct Stack* stack);
struct expr * peek(struct Stack* stack);
void cleanupStack(struct Stack* stack);
struct expr * remove_redundant_paren(struct expr * e);
int is_atomic(struct expr * e);
int is_var_expr(struct expr * e);

#endif