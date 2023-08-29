#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void usage();
int char_in_list(char, char [], int);
int backslash_count_back(const char *, int);
int run_encode(const char*, unsigned int);

#endif