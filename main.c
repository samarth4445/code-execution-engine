#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "docker.h"

#define DOCKER_FILE_PATH "./dockerfiles/"

void print_help();

int main(int argc, char *argv[]) {

    if(argc == 2){
        if(strncmp(argv[1], "--help", 6) == 0){
                print_help();
        }

        exit(EXIT_SUCCESS);
    }

    if(argc < 6){
        printf("Too few arguments \n");
        exit(EXIT_FAILURE);
    }

    struct Command command;

    command.lang = argv[1];
    command.code_file_name = argv[2];
    command.code_input_file_name = argv[3];
    command.code_output_file_name = argv[4];
    command.cwd = argv[5];

    build_command(&command);
    printf("%s\n", command.command);
    
    run_container(&command);
}

void print_help(){
    printf("Command structure should be like the following command.\n");
    printf("./cce language-name /absolute/path/to/code-file /absolute/path/to/input-file /path/to/output-file \n");
}
