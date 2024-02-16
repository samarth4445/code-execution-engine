#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "docker.h"

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

void build_command(struct Command *lang){
    char command[600];
    command[0] = '\0';

    if(strcmp(lang->lang, "python") == 0) {
        strncat(command, "python3 ", 500);
        strncat(command, lang->code_file_name, 500);

        if(strncmp(lang->code_input_file_name, "null", 4) != 0) {
            strncat(command, " < ", 500);
            strncat(command, lang->code_input_file_name, 500);
            strncat(command, " ", 500);
        }

        if (strncmp(lang->code_output_file_name, "null", 4) == 0) {
            printf("Please enter an output file!\n");
            exit(EXIT_FAILURE);
        } else {
            strncat(command, " > ", 500);
            strncat(command, lang->code_output_file_name, 500);
            strncat(command, " ", 500);
        }
    }

    if(strcmp(lang->lang, "c") == 0){
        strncat(command, "gcc ", 500);
        strncat(command, lang->code_file_name, 500);

        strncat(command, " && ./a.out ", 500);

        if(strncmp(lang->code_input_file_name, "null", 4) != 0) {
            strncat(command, "< ", 500);
            strncat(command, lang->code_input_file_name, 500);
            strncat(command, " ", 500);
        }

        if (strncmp(lang->code_output_file_name, "null", 4) == 0) {
            printf("Please enter an output file!\n");
            exit(EXIT_FAILURE);
        } else {
            strncat(command, " > ", 500);
            strncat(command, lang->code_output_file_name, 500);
            strncat(command, " ", 500);
        }
    }

    if(strcmp(lang->lang, "cpp") == 0){
        strncat(command, "g++ ", 500);
        strncat(command, lang->code_file_name, 500);

        strncat(command, " && ./a.out ", 500);

        if(strncmp(lang->code_input_file_name, "null", 4) != 0) {
            strncat(command, "< ", 500);
            strncat(command, lang->code_input_file_name, 500);
            strncat(command, " ", 500);
        }

        if (strncmp(lang->code_output_file_name, "null", 4) == 0) {
            printf("Please enter an output file!\n");
            exit(EXIT_FAILURE);
        } else {
            strncat(command, " > ", 500);
            strncat(command, lang->code_output_file_name, 500);
            strncat(command, " ", 500);
        }
    }

    if(strcmp(lang->lang, "golang") == 0){
        
    }

    strcpy(lang->command, command);
}

int get_user_id(struct Command *lang, int command_to_server){
    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(data_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path));

    ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un));

    if(ret == -1){
        fprintf(stderr, "The server is down \n");
        exit(EXIT_FAILURE);
    }

    i = command_to_server;
    ret = write(data_socket, &i, sizeof(int));

    if(ret == -1){
        perror("write");
        exit(EXIT_FAILURE);
    }

    printf("No of bytes send = %d, data sent = %d\n", ret, i);

    memset(buffer, 0, BUFFER_SIZE);
    ret = read(data_socket, buffer, BUFFER_SIZE);
    if(ret == -1){
        perror("read");
        exit(EXIT_FAILURE);
    }

    buffer[BUFFER_SIZE-1] = '\0';
    close(data_socket);

    return atoi(buffer);
}

void run_container(struct Command *command) {
    FILE *fp;
    char buffer[1024];
    char *output = NULL;
    size_t output_size = 0;

    char grep_command[] = "docker container ls | grep code-1";

    fp = popen(grep_command, "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t buffer_size = strlen(buffer);
        output = realloc(output, output_size + buffer_size + 1); 
        if (output == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        strcpy(output + output_size, buffer);
        output_size += buffer_size;
    }
    pclose(fp);

    if (output == NULL) {
        output = strdup("");
        if (output == NULL) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
    }

    if (strlen(output) == 0) {
        printf("Container is not running. Starting the container with your code!!\n");

        int user_id = get_user_id(command, 0);
        printf("Running the code in the container with user id %d. \n", user_id);

        char docker_command_buffer[1024];
        sprintf(docker_command_buffer, "docker container run --rm -v %s:/home/app/ -it -u user%d code-1 sh -c \"%s\" ", command->cwd , user_id, command->command);
        system(docker_command_buffer);

    } else {
        printf("Running your code in the container! \n");

        int user_id = get_user_id(command, 0);

        printf("Running the code in the container with user id. %d \n", user_id);
    }

    free(output);
}