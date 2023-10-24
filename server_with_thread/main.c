// Include necessary libraries and headers
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>

// Prototype for the connection handler function
void *connection_handler(void *);

int main() {
    // Initialize a message that the server will send to clients 
    char server_message[100] = "Hello from Server!!\n";

    // Declare the server socket variable
    int server_socket;

    // Create teh server socker of type IPV4 and TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Check if the server socker was create successfully
    if (server_socket == -1) {
	// Print the error message
        perror("Socket initialisation failed");
	// Exit the program with failure status
        exit(EXIT_FAILURE);
    } else {
	// Inform that the socker was created
        printf("Server socket created successfully\n");
    }

    // Structure to hold the server's IP address and port information
    struct sockaddr_in server_addr;
    // Using IPv4
    server_addr.sin_family = AF_INET;
    // Set port number (8080) and convert to network byte order
    server_addr.sin_port = htons(8080);
    // Use any available interface of the machine
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified IP address and port
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    } else {
        printf("Socket successfully binded..\n");
    }

    // Listen for incoming connections, allow a backlog of 3
    if (listen(server_socket, 3) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else {
        printf("Server listening..\n");
    }

    // Counter for number of threads (or clients) handled
    int no_threads = 0;
    // Array to hold thread IDs
    pthread_t threads[3];

    // Keep accepting client connections until 3 clients have connected
    while (no_threads < 3) {
        printf("Listening...\n");
	// Accept an incoming client connection
        int client_socket = accept(server_socket, NULL, NULL);
        puts("Connection accepted");
        
	// Create a new thread to handle the client's request
        if (pthread_create(&threads[no_threads], NULL, connection_handler, &client_socket) < 0) {
	    // Error creating thread
            perror("Could not create thread");
            return 1;
        }

	// Check if the client was accepted successfully
        if (client_socket < 0) {
            printf("Server accept failed...\n");
            exit(0);
        } else {
            printf("Server accepted the client...\n");
	    // A thread has been assigned to a handle the client
            puts("Handler assigned");
	    // Increment the number of threads/clients handled
            no_threads++;
        }
    }

    // Wait for all threads to finish executing 
    for (int k = 0; k < 3; k++) {
        pthread_join(threads[k], NULL);
    }

    // Close the server socket
    close(server_socket);

    // Close the server socket
    return 0;
}

// Function to handle client requests
void *connection_handler(void *client_socket) {
    // Extract the socket file descriptor
    int socket = *(int*) client_socket;
    int read_len;
    // Initialize a server message
    char server_message[100] = "Hello from server\n";
    int send_status;

    // Send the initial message to the client
    send_status = send(socket, server_message, sizeof(server_message), 0);
    // Buffer to hold client's message
    char client_message[100];
    
    while ((read_len = recv(socket, client_message, 100, 0)) > 0) {
        // End of string marker
        client_message[read_len] = '\0';
        if (strcmp(client_message, "exit") == 0) {
            break;
        }
        // Send the message back to the client
        send_status = send(socket, client_message, strlen(client_message), 0);
    }

    // End the thread
    return 0;
}

