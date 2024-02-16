#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int main(int argc, char *argv[]){
    struct sockaddr_un name;

    #if 0
        struct sockaddr_un {
            sa_family_t sun_family;
            char sun_path[108];
        }
    #endif

    int ret;
    int connection_socket;
    int data_socket;
    int result;
    int data;
    int users_available[14];
    char buffer[BUFFER_SIZE];

    
    for(int j=0; j<14; j++){
        users_available[j] = 1;
    }

    unlink(SOCKET_NAME);

    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(connection_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Master socket created \n");
    memset(&name, 0, sizeof(struct sockaddr_un));

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path)-1);

    ret = bind(connection_socket, (const struct sockaddr *) &name, sizeof(struct sockaddr_un));

    if(ret == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("bind() call succeed \n");

    ret = listen(connection_socket, 20);
    if(ret == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Waiting on accept() sys call \n");

        data_socket = accept(connection_socket, NULL, NULL);

        if(data_socket == -1){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted from client \n");
        memset(buffer, 0, BUFFER_SIZE);
        
        printf("Waiting for data from the client \n");
        ret = read(data_socket, buffer, BUFFER_SIZE);

        if(ret == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }

        memcpy(&data, buffer, sizeof(int));

        int userID = -1;
        
        if(data == 0){
            memset(buffer, 0, BUFFER_SIZE);
            
            for(int j=0; j<14; j++){
                if(users_available[j] == 1){
                    users_available[j] = 0;
                    userID = j+1;
                    break;
                }
            }
            
            sprintf(buffer, "%d", userID);

            printf("Sending final result back to the client \n");
            ret = write(data_socket, buffer, BUFFER_SIZE);

            if(ret == -1){
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        else if(data > 0){
            printf("Freeing space for user id %d\n", data);

            users_available[data-1] = 1;

            sprintf(buffer, "Freed space for user id %d\n", data);
            printf("%s\n", buffer);

            printf("Sending final result back to the client \n");
            ret = write(data_socket, buffer, BUFFER_SIZE);

            memset(buffer, 0, BUFFER_SIZE);


            if(ret == -1){
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(data_socket);
    }

    close(connection_socket);
    printf("connection closed .. \n");

    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}
