#include "encode.h"

int string_decode(const char *es, char *s){
    char codes[NUMBER_CODES] = {'a', 'b', 'e', 'n', 'r', 't', 'f', 'v', '\\', '\'', '\"'};
    char ascii_chars[NUMBER_CODES] = {'\a', '\b', '\e', '\n', '\r', '\t', '\f', '\v', '\\', '\'', '\"'};
    size_t es_len = strlen(es);

    if(es_len < 2) return 0; // cannot be valid if we dont have double quotes aka len < 2

    if(es[0] != '"' || es[es_len - 1] != '"'){  // String is not surrounded by double quotes
        printf("String not surrounded by double quotes\n");
        return 0;
    }

    if(backslash_count_back(es, es_len - 2)){ //check we have all backslashes from the back
        printf("Odd number of double backslashes starting from the back of string\n");
        return 0; 
    }
    
    int i;
    int s_counter = 0;
    for(i = 1; i < es_len - 1; i++){ // skip first quote and last quote
        if(es[i] == '\\'){ //found backslash
            char next_char = es[i + 1];

            if(next_char == '0'){ // we have hex
                char x_char = (i + 2) < es_len - 1 ? es[i + 2] : 0;
                char ms_hex = (i + 3) < es_len - 1 ? es[i + 3] : 0;
                char ls_hex = (i + 4) < es_len - 1 ? es[i + 4] : 0;

                if(x_char == 'x' && isxdigit(ms_hex) && isxdigit(ls_hex)){
                    char hex[3];
                    hex[0] = ms_hex;
                    hex[1] = ls_hex;
                    hex[2] = '\0';
                    char c = (char)strtol(hex, NULL, 16);

                    if(c < 32 && c > 136) return 0;

                    s[s_counter] = c;
                    i += 4; // skip hex sequence
                }
                else{ //invalid hex
                    printf("Hex format incorrect to decode\n");
                    return 0;
                }
            }
            else{ // check regular escapes
                int ascii_code_type = char_in_list(next_char, codes, NUMBER_CODES);
                if(ascii_code_type == -1) {
                    printf("Invalid escape character\n");
                    return 0;
                }
                s[s_counter] = ascii_chars[ascii_code_type];
                i++; // skip this escape character
            }
        }
        else if(es[i] == '\"' && es[i - 1] != '\\'){ // " in the middle of a string
            printf("Double quote in the middle of the string\n");
            return 0;
        }
        else if(es[i] >= 32 && es[i] <= 136) s[s_counter] = es[i]; // valid ascii between 32 and 136 inclusive
        else {
            printf("Not ASCII character between 32 and 136 inclusive\n");
            return 0;
        }
        s_counter++;
    }

    s[s_counter] = '\0';

    return 1;
}

int string_encode(const char *s, char *es){
    char *encoded_str = (char *)malloc(256 * sizeof(char));

    if(!encoded_str){
        perror("Malloc failed");  
        return 0;
    }

    char codes[NUMBER_CODES] = {'a', 'b', 'e', 'n', 'r', 't', 'f', 'v', '\\', '\'', '\"'};
    char ascii_chars[NUMBER_CODES] = {'\a', '\b', '\e', '\n', '\r', '\t', '\f', '\v', '\\', '\'', '\"'};
    size_t s_len = strlen(s);

    encoded_str[0] = '"'; //start with a double quote

    int i;
    int encoded_str_cnt = 1;
    for(i = 0; i < s_len; i++){
        int ascii_code_type;
        if( (ascii_code_type = char_in_list(s[i], ascii_chars, NUMBER_CODES)) != - 1){
            encoded_str[encoded_str_cnt] = '\\';
            encoded_str[encoded_str_cnt + 1] = codes[ascii_code_type];
            encoded_str_cnt++; // we added two characters, skip the next one so we're back on track
        }
        else{
            encoded_str[encoded_str_cnt] = s[i];
        }
        encoded_str_cnt++;
    }

    encoded_str[encoded_str_cnt] = '"'; // add final quote

    encoded_str[encoded_str_cnt + 1] = '\0'; // end with a null

    if(strcmp(encoded_str, es)){ // new encoded different than original encoded
        free(encoded_str);
        printf("New encoded string different than original encoded\n");
        return 0; 
    }

    free(encoded_str);
    return 1;
}

int encode(FILE *file){
    char buffer[256];
    char *output_encoded_str = NULL;
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *input_encoded_str = buffer;
        output_encoded_str = (char *) malloc(256 * sizeof(char));

        if(!output_encoded_str){
            perror("Malloc failed");  
            return EXIT_FAILURE;
        }

        if(string_decode(input_encoded_str, output_encoded_str) && string_encode(output_encoded_str, input_encoded_str)){
            printf("Encode Decode Process Successful!\n");
            printf("Encoded str: %s\n", input_encoded_str);
            printf("Decoded str: %s\n", output_encoded_str);
            free(output_encoded_str);
            return EXIT_SUCCESS;
            
        }
        else{
            printf("Error decoding string: %s\n", buffer);
            free(output_encoded_str);
            return EXIT_FAILURE;
        }

    } 
    else {
        printf("The file is empty.\n");
        return EXIT_FAILURE;
    }
}

int run(const char* file_name, unsigned int flags){
    int run_status = EXIT_FAILURE;

    if(file_name == NULL){
        printf("No file name passed as argument\n");
        return run_status;
    }

    FILE *file;
    file = fopen(file_name, "r");

    if (!file) {
        perror("Error opening the file");
        return run_status;
    }

    if(flags & ENCODE_FLAG) run_status = encode(file);


    fclose(file);

    return run_status;
}