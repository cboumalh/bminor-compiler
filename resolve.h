#ifndef RESOLVE_H
#define RESOLVE_H

#include "decl.h"
#include "scope_result.h"
#include <stdlib.h>
#include "resolve_result.h"

int resolve(struct decl *ast, int verbose){
    sc = scope_enter();
    decl_resolve(ast, verbose);
    scope_exit();
    if(!resolve_result)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


#endif