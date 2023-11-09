//libraries
#include "encode.h"
#include "scan.h"
#include "parse.h"
#include "print.h"
#include "resolve.h"
#include "typecheck.h"
#include "parser_result.h"
#include "resolve_result.h"
#include "scope_result.h"
#include "typecheck_result.h"

struct scope *sc = NULL;
int resolve_result = 1;
int typecheck_result = 1;

int main(int argc, char *argv[]){

    int arg_counter = 1;
    char *input_file_name = NULL;
    unsigned int flags = 0;
    int run_status = EXIT_SUCCESS;

    if(argc < 2){
        usage();
        run_status = EXIT_FAILURE;
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
            if(!strcmp(flag_no_dashes, "parse")){
                flags |= PARSE_FLAG;
            }
            if(!strcmp(flag_no_dashes, "print")){
                flags |= PRINT_FLAG;
            }
            if(!strcmp(flag_no_dashes, "resolve")){
                flags |= RESOLVE_FLAG;
            }
            if(!strcmp(flag_no_dashes, "typecheck")){
                flags |= TYPECHECK_FLAG;
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
        run_status = EXIT_FAILURE;
        return run_status;
    }

    FILE *file;
    file = fopen(input_file_name, "r");

    if (!file) {
        perror("Error opening the file");
        run_status = EXIT_FAILURE;
        return run_status;
    }

    if(flags & ENCODE_FLAG) run_status = encode(file);
    if(flags & SCAN_FLAG && run_status == EXIT_SUCCESS) run_status = scan(file);
    if((flags & PARSE_FLAG || flags & PRINT_FLAG || flags & RESOLVE_FLAG || flags & TYPECHECK_FLAG) && (run_status == EXIT_SUCCESS)) run_status = parse(file);
    if(flags & PRINT_FLAG && run_status == EXIT_SUCCESS) run_status = print(parser_result);
    if((flags & RESOLVE_FLAG || flags & TYPECHECK_FLAG) && run_status == EXIT_SUCCESS) run_status = resolve(parser_result);
    if(flags & TYPECHECK_FLAG && run_status == EXIT_SUCCESS) run_status = typecheck(parser_result);

    fclose(file);

    if(input_file_name)
        free(input_file_name);

    return run_status;
}