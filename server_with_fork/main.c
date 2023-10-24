#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

// Function prototypes
void connection_handler(int socket);

int main() {
    char server_message[100] = "Hello from Server!!\n";
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        perror("Socket initialisation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Server socket created successfully\n");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    } else {
        printf("Socket successfully binded..\n");
    }

    if (listen(server_socket, 3) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else {
        printf("Server listening..\n");
    }

    // Continuously accept client connections
    while (1) {
        printf("Listening...\n");
        int client_socket = accept(server_socket, NULL, NULL);
        
        if (client_socket < 0) {
            perror("Server accept failed");
            continue;  // Move on to the next iteration
        }
        
        // When a new client connects, fork a new process to handle it
        pid_t pid = fork();

        if (pid == 0) {  // This block will be executed by the child process
            close(server_socket);  // Close the listening socket in the child
            connection_handler(client_socket);  // Process the client's request
            close(client_socket);
            exit(0);  // Child process ends
        } else if (pid > 0) {  // This block will be executed by the parent process
            close(client_socket);  // Close the client socket in the parent so that only the child will communicate with the client
        } else {
            perror("Fork failed");  // Error handling in case fork() fails
            close(client_socket);
        }
    }

    // Close the server socket (though we'll likely never reach here due to the infinite loop)
    close(server_socket);
    
    return 0;
}

void connection_handler(int socket) {
    int read_len;
    char server_message[100] = "Hello from server\n";
    
    // Send the initial message to the client
    send(socket, server_message, sizeof(server_message), 0);
    
    char client_message[100];

    // Continuously read messages from the client
    while ((read_len = recv(socket, client_message, 100, 0)) > 0) {
        client_message[read_len] = '\0';
        
        if (strcmp(client_message, "exit") == 0) {
            break;
        }
        
        send(socket, client_message, strlen(client_message), 0);
    }
}

