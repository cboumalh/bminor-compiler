#include "decl.h"

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

void indent_print(int indent){
    int i;
    for(i = 0; i < indent; i++)
        printf(" ");
}