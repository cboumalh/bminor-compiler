#ifndef SCAN_H
#define SCAN_H

#include "lex.yy.c"
#include <stdlib.h>
#include "token.h"

int scan(FILE* file){
    yyin = file;
    while(1){
        int token = yylex();
        if(!token) break;
        if (token == ERROR_TOKEN) return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}


#endif