//libraries
#include "encode.h"


int main(int argc, char *argv[]){

    int arg_counter = 1;
    char *input_file_name = NULL;
    unsigned int flags = 0;

    if(argc < 2){
        usage();
        return EXIT_FAILURE;
    }

    while(arg_counter < argc){
        //for flags
        if(strlen(argv[arg_counter]) > 2 && argv[arg_counter][0] == '-' && argv[arg_counter][1] == '-'){
            char *flag_no_dashes = argv[arg_counter] + 2;

            if(!strcmp(flag_no_dashes, "encode")){
                flags |= ENCODE_FLAG;
            }
        }
        //input file
        else{
            input_file_name = strdup(argv[arg_counter]);
        }

        arg_counter++;
    }

    int run_status = run(input_file_name, flags);

    if(input_file_name)
        free(input_file_name);

    return run_status;
}