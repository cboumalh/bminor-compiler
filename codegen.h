#ifndef CODEGEN_H
#define CODEGEN_H

#include "decl.h"
#include "typecheck_result.h"
#include <stdlib.h>


int codegen(struct decl *ast, FILE *output_file, char *output_file_name){
    if(!output_file) return EXIT_FAILURE;
    fprintf(output_file, ".file \"%s\"\n", output_file_name);
    decl_codegen(ast, output_file);

    return EXIT_SUCCESS;
}


#endif