#ifndef PARSE_H
#define PARSE_H

#include "parser.tab.h"
#include "parser.tab.c"
#include <stdlib.h>

int parse(FILE* file){
    yyin = file;
    yyparse();
    return EXIT_SUCCESS;
}


#endif