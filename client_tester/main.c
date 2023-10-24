#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"  // Server IP
#define SERVER_PORT 8080       // Server port
#define MAX_THREADS 100        // Maximum number of threads to test with

void* connection_thread(void* arg) {
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Could not create socket");
        return NULL;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed. Server might be unresponsive or limit reached");
        close(client_socket);
        return NULL;
    }

    // Keep the connection alive
    while (1) {
        sleep(10);  // Sleep to prevent busy-wait and simulate a long-lasting connection
    }

    close(client_socket);
    return NULL;
}

int main() {
    pthread_t threads[MAX_THREADS];
    int i;

    // Create threads and initiate connections to server
    for (i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, connection_thread, NULL) != 0) {
            perror("Failed to create thread");
        }
        sleep(1);  // Small sleep to ensure server gets hit sequentially
    }

    // Wait for all threads to complete
    for (i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

