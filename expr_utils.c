#include "expr_utils.h"

void initializeStack(struct Stack* stack) {
    stack->top = NULL;
}

int isEmpty(struct Stack* stack) {
    return stack->top == NULL;
}

void push(struct Stack* stack, struct expr *value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        return;
    }
    newNode->data = value;
    newNode->next = stack->top;
    stack->top = newNode;
}

struct expr * pop(struct Stack* stack) {
    if (isEmpty(stack)) {
        return NULL; 
    } else {
        struct Node* topNode = stack->top;
        struct expr * value = topNode->data;
        stack->top = topNode->next;
        free(topNode);
        return value;
    }
}

struct expr * peek(struct Stack* stack) {
    if (isEmpty(stack)) {
        return NULL; 
    } else {
        return stack->top->data;
    }
}

void cleanupStack(struct Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}

int is_atomic(struct expr * e){
    if(!e) return 0;

    int atomic_types [] = {EXPR_INTEGER_LITERAL, EXPR_STRING_LITERAL, EXPR_BOOL_LITERAL, EXPR_FLOAT_LITERAL, EXPR_NAME, EXPR_CHAR_LITERAL, EXPR_SUBSCRIPT, EXPR_CALL, EXPR_ONE_D_ARR, EXPR_ARRAY_DECL, EXPR_ARG};
    int i;

    for(i = 0; i < sizeof(atomic_types)/sizeof(atomic_types[0]); i++)
        if(e->kind == atomic_types[i])
            return 1;

    return 0;
}

// remove redundant parantheses
struct expr* remove_redundant_paren(struct expr * e){
	if(!e) return e;

	struct Stack myStack;
    initializeStack(&myStack);
    int precedence[] = {0, 1, 2, 3, 3, 3, 3, 3, 3, 4, 4, 5, 5, 5, 6, 7, 7, 8, 8};
	
    while(e && e->kind == EXPR_PARAN)
        e = e->right;

    push(&myStack, e);
    
    while(!isEmpty(&myStack)){

        struct expr *top = pop(&myStack);
        if(is_atomic(top)) continue;

        struct expr *left = top->left;
        struct expr *right = top->right;

        // remove any excess parantheses inside other parantheses
        while(left && left->kind == EXPR_PARAN && left->right->kind == EXPR_PARAN)
            left->right = left->right->right;

        while(right && right->kind == EXPR_PARAN && right->right->kind == EXPR_PARAN)
            right->right = right->right->right;
        
        // remove parantheses around atomics
        if(right && right->kind == EXPR_PARAN && is_atomic(right->right)) 
            top->right = right->right;
        if(left && left->kind == EXPR_PARAN && is_atomic(left->right))
            top->left = left->right;

        //check precedences and remove parantheses based on it
        left = top->left;
        right = top->right;

        if(right && right->kind == EXPR_PARAN && precedence[top->kind] < precedence[right->right->kind])
            top->right = right->right;
        if(left && left->kind == EXPR_PARAN && precedence[top->kind] < precedence[left->right->kind])
            top->left = left->right;


        if(top->left) push(&myStack, top->left);
        if(top->right) push(&myStack, top->right);
    }

    return e;
}

int is_var_expr(struct expr * e){
    if(!e) return 0;
    
    if(e->kind == EXPR_NAME) return 1;

    return is_var_expr(e->left) || is_var_expr(e->right); 
}