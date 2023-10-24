#ifndef PRINT_H
#define PRINT_H

#include "decl.h"
#include <stdlib.h>


int print(struct decl *parser_result){
    decl_print(parser_result, 4);
    return EXIT_SUCCESS;
}


#endif