#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
  int sockfd;
  int n;
  struct sockaddr_in servaddr;
  char buf[1024];

  // Create a socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    exit(1);
  }

  // Connect to the server
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8080);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("connect");
    exit(1);
  }

  // Write a message to the server
  strcpy(buf, "Hello, world!");
  n = write(sockfd, buf, strlen(buf));
  if (n < 0) {
    perror("write");
    exit(1);
  }

  // Read a message from the server
  n = read(sockfd, buf, sizeof(buf));
  if (n < 0) {
    perror("read");
    exit(1);
  }

  // Print the message
  printf("Received message: %s\n", buf);

  // Close the socket
  close(sockfd);

  return 0;
}
