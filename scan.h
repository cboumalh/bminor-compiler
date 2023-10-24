#ifndef SCAN_H
#define SCAN_H

#include "lex.yy.c"
#include <stdlib.h>
#include "token.h"

#define TOKEN_START 258

int scan(FILE* file){
    yyin = file;
    while(1){
        int token = yylex();
        if(!token) break;
        if (token == ERROR_TOKEN) {
            return EXIT_FAILURE;
        }
        
        printf("%s\n", TokenNames[token - TOKEN_START]);
    }
    printf("Scan Succeeded\n");
    return EXIT_SUCCESS;
}


#endif