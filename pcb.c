#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Define the IPv6 address of PCB:
#define DEST_ADDR "2001:2::2"
// Define the port number to receive packets on:
#define PORT_NUM 12345

int main() {
  // Create a socket to receive IPv6 packets:
  int sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
  if (sockfd < 0) {
      perror("socket");
      exit(1);
  }

  // Bind the socket to the IPv6 address of PCB and the port number:
  struct sockaddr_in6 src_addr;
  memset(&src_addr, 0, sizeof(src_addr));
  src_addr.sin6_family = AF_INET6;
  src_addr.sin6_addr = in6addr_any;
  src_addr.sin6_port = htons(PORT_NUM);
  if (bind(sockfd, (struct sockaddr*)&src_addr, sizeof(src_addr)) < 0) {
      perror("bind");
      exit(1);
  }

  // Set the destination IPv6 address:
  struct sockaddr_in6 dest_addr;
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin6_family = AF_INET6;
  dest_addr.sin6_port = htons(0);
  if (inet_pton(AF_INET6, DEST_ADDR, &dest_addr.sin6_addr) != 1) {
      fprintf(stderr, "Invalid address\n");
      exit(1);
  }

  // Wait for a packet to arrive:
  char buffer[1024];
  struct sockaddr_in6 sender_addr;
  socklen_t sender_addrlen = sizeof(sender_addr);
  int num_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&sender_addr, &sender_addrlen);
  if (num_bytes < 0) {
      perror("recvfrom");
      exit(1);
  }

  // Print the received message:
  printf("Received message from PCA: %s\n", buffer);

  // Close the socket:
  close(sockfd);

  return 0;
}
