#ifndef PARSE_H
#define PARSE_H

#include "parser.tab.h"
#include "parser.tab.c"
#include "scan.h"
#include <stdlib.h>
#include "token.h"

int parse(FILE* file){
    yyin = file;
    yyparse();
    return EXIT_SUCCESS;
}


#endif