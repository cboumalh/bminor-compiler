#include "utils.h"

int char_in_list(char c, char arr[], int sz){
    for(int j = 0; j < sz; j++)
        if(c == arr[j])  return j;

    return -1;
}

void usage(){
    printf("Usage:\n./bminor --flags input.bminor\n");
}

int backslash_count_back(const char *es, int end){
    int backslash_count = 0;

    while(end >= 0 && es[end] == '\\') {
        backslash_count++;
        end--;
    }

    if(backslash_count % 2) return 1;

    return 0;  
}