#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Define the IPv6 address of PCB:
#define DEST_ADDR "2001:3::1"
// Define the IPv6 address of PCA:
#define SRC_ADDR "2001:2::1"
// Define the tunnel interface name:
#define TUNNEL_INTERFACE "yyyyyy"

int main() {

  // Create a socket to send IPv6 packets:
  int sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
  if (sockfd < 0) {
      perror("socket");
      exit(1);
  }

  // Bind the socket to the source IPv6 address:
  struct sockaddr_in6 src_addr;
  memset(&src_addr, 0, sizeof(src_addr));
  src_addr.sin6_family = AF_INET6;
  if (inet_pton(AF_INET6, SRC_ADDR, &src_addr.sin6_addr) != 1) {
      fprintf(stderr, "Invalid address\n");
      exit(1);
  }
  src_addr.sin6_port = 0;
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

  // Set the source IPv6 address:
  struct sockaddr_in6 src_tunnel_addr;
  memset(&src_tunnel_addr, 0, sizeof(src_tunnel_addr));
  src_tunnel_addr.sin6_family = AF_INET6;
  if (inet_pton(AF_INET6, "2001:2::2", &src_tunnel_addr.sin6_addr) != 1) {
      fprintf(stderr, "Invalid address\n");
      exit(1);
  }
  src_tunnel_addr.sin6_port = 0;
  if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_PKTINFO, &src_tunnel_addr, sizeof(src_tunnel_addr)) < 0) {
      perror("setsockopt");
      exit(1);
  }

  // Send a packet to PCB:
  char* message = "Hello, PCB!";
  if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
      perror("sendto");
      exit(1);
  }

  // Close the socket:
  close(sockfd);

}
