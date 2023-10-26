#ifndef PRINT_H
#define PRINT_H

#include "decl.h"
#include <stdlib.h>


int print(struct decl *parser_result){
    
    while(parser_result){
        decl_print(parser_result, INDENT);
        parser_result = parser_result->next;
    }

    return EXIT_SUCCESS;
}


#endif