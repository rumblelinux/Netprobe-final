#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int sockfd, optval = 1;
    struct sockaddr_in server_addr, client_addr;
    struct sockaddr_in6 client_addr6;
    socklen_t addrlen = sizeof(client_addr);
    socklen_t addrlen6 = sizeof(client_addr6);
    char buffer[BUF_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket option to reuse address and port
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to local address and port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(5000);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("PCB is ready to receive IPv6 traffic from PCA\n");

    // Receive data from socket
    while (1) {
        int n = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr6, &addrlen6);
        if (n < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        // Print the received data
        printf("PCB received %d bytes from %s: ", n, inet_ntoa(client_addr.sin_addr));
        for (int i = 0; i < n; i++) {
            printf("%02X ", (unsigned char)buffer[i]);
        }
        printf("\n");
    }

    close(sockfd);
    return 0;
}
