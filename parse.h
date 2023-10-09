#ifndef PARSE_H
#define PARSE_H

#include "parser.tab.h"
#include "parser.tab.c"
#include <stdlib.h>

int parse(FILE* file){
    yyin = file;
    if(yyparse() == 0)
        printf("Parse Succeeded\n");
        return EXIT_SUCCESS;

    printf("Parse Failed\n");
    return EXIT_FAILURE;
}


#endif