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

    return backslash_count % 2;
}

void toUpperCase(const char *input, char *output) {
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        output[i] = toupper(input[i]);
    }
    
    output[len] = '\0'; // Null-terminate the output string
}