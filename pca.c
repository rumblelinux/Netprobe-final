#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
  int sockfd;
  struct sockaddr_in servaddr;
  char buf[1024];

  // Create a socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    exit(1);
  }

  // Bind the socket to a port
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8080);
  servaddr.sin_addr.s_addr = INADDR_ANY;
  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind");
    exit(1);
  }

  // Listen for connections
  listen(sockfd, 5);

  // Accept a connection
  int clientfd = accept(sockfd, NULL, NULL);
  if (clientfd < 0) {
    perror("accept");
    exit(1);
  }

  // Read a message from the client
  int n = read(clientfd, buf, sizeof(buf));
  if (n < 0) {
    perror("read");
    exit(1);
  }

  // Print the message
  printf("Received message: %s\n", buf);

  // Close the socket
  close(sockfd);
  close(clientfd);

  return 0;
}
