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

char strtochar(const char* str){
    char codes[NUMBER_CODES] = {'a', 'b', 'e', 'n', 'r', 't', 'f', 'v', '\\', '\'', '\"'};
    char ascii_chars[NUMBER_CODES] = {'\a', '\b', '\e', '\n', '\r', '\t', '\f', '\v', '\\', '\'', '\"'};
    int length = strlen(str);

    if(length == 3 && char_in_list(str[1], ascii_chars, NUMBER_CODES) == -1) return str[1];

    else if (length == 4) return ascii_chars[char_in_list(str[2], codes, NUMBER_CODES)];

    else if(length == 7){
        char hex[3];
        hex[0] = str[4];
        hex[1] = str[5];
        hex[2] = '\0';
        return (char)strtol(hex, NULL, 16);
    }
    else{
        return '\0';
    }
}

void indent_print(int indent){
    int i;
    for(i = 0; i < indent; i++)
        printf(" ");
}