#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NUMBER_CODES 12
#define INDENT 4


//flags
enum Flags {
    ENCODE_FLAG = 1 << 0,
    SCAN_FLAG = 1 << 1,
    PARSE_FLAG = 1 << 2,
    PRINT_FLAG = 1 << 3,
    RESOLVE_FLAG =  1 << 4,
    TYPECHECK_FLAG =  1 << 5
};



void usage();
int char_in_list(char, char [], int);
int backslash_count_back(const char *, int);
void toUpperCase(const char *, char *); 
char strtochar(const char *);
void indent_print(int);



#endif