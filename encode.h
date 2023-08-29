#ifndef ENCODE_H
#define ENCODE_H

#include "utils.h"

//macros
#define NUMBER_CODES 12

//flags
enum Flags {
    ENCODE_FLAG = 1 << 0
};

//prototypes
int string_decode( const char *, char * );
int string_encode( const char *, char * );
int run(const char*, unsigned int);

#endif