#ifndef ENCODE_H
#define ENCODE_H

#include "utils.h"

//macros
#define NUMBER_CODES 12
#define BUFSIZE 256

//prototypes
int string_decode( const char *, char * );
int string_encode( const char *, char * );
int encode(FILE *);

#endif