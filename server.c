#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_TEXT_LENGTH 1024

struct packet {
    char text[MAX_TEXT_LENGTH];
    char sNummer[10];
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server-port> <sNummer>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse command line arguments
    const int server_port = atoi(argv[1]);
    const char *sNummer = argv[2];

    // Create a TCP socket
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    struct sockaddr_in6 server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(server_port);
    server_addr.sin6_addr = in6addr_any;

    // Bind the socket to a specific port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("Error listening for connections");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    printf("Waiting for incoming connection...\n");
    struct sockaddr_in6 client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_sockfd == -1) {
        perror("Error accepting connection");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connection established with the client.\n");

    // Main loop
    while (1) {
        // Set up file descriptor set for select()
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(client_sockfd, &read_fds);  // Add client socket

        // Use select() to check for available input
        if (select(client_sockfd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Error in select()");
            close(client_sockfd);
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Check if there is incoming data from the client
        if (FD_ISSET(client_sockfd, &read_fds)) {
            // Receive and display data
            struct packet recv_packet;
            memset(&recv_packet, 0, sizeof(recv_packet));
            ssize_t bytes_received = recv(client_sockfd, &recv_packet, sizeof(recv_packet), 0);
            if (bytes_received <= 0) {
                if (bytes_received == 0) {
                    printf("Client closed the connection.\n");
                } else {
                    perror("Error receiving data");
                }
                close(client_sockfd);
                close(sockfd);
                exit(EXIT_FAILURE);
            } else {
                printf("Received from client: %s > %s\n", recv_packet
