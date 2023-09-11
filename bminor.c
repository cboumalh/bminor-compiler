//libraries
#include "encode.h"
#include "scan.h"

int main(int argc, char *argv[]){

    int arg_counter = 1;
    char *input_file_name = NULL;
    unsigned int flags = 0;
    int run_status = EXIT_FAILURE;

    if(argc < 2){
        usage();
        return run_status;
    }

    while(arg_counter < argc){
        //for flags
        if(strlen(argv[arg_counter]) > 2 && argv[arg_counter][0] == '-' && argv[arg_counter][1] == '-'){
            char *flag_no_dashes = argv[arg_counter] + 2;

            if(!strcmp(flag_no_dashes, "encode")){
                flags |= ENCODE_FLAG;
            }
            if(!strcmp(flag_no_dashes, "scan")){
                flags |= SCAN_FLAG;
            }
        }
        //input file
        else{
            input_file_name = strdup(argv[arg_counter]);
        }

        arg_counter++;
    }

    if(input_file_name == NULL){
        printf("No file name passed as argument\n");
        return run_status;
    }

    FILE *file;
    file = fopen(input_file_name, "r");

    if (!file) {
        perror("Error opening the file");
        return run_status;
    }

    if(flags & ENCODE_FLAG) run_status = encode(file);
    if(flags & SCAN_FLAG) run_status = scan(file);


    fclose(file);

    if(input_file_name)
        free(input_file_name);

    return run_status;
}