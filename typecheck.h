#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "decl.h"
#include "typecheck_result.h"
#include <stdlib.h>


int typecheck(struct decl *ast){
    decl_typecheck(ast);

    if(!typecheck_result)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


#endif