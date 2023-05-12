#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#include <errno.h>

#define SRC_IP "fe80::41e6:a4db:49b1:8934%12"
#define DST_IP "fe80::9cb9:c83c:7ffe:fab6%20"
#define SRC_PORT 9000
#define DST_PORT 9001
#define MSG "Hello from PCA!"

int main()
{
    int sockfd, rc;
    struct sockaddr_in6 src_addr, dst_addr;
    struct ip6_hdr iphdr;
    struct icmp6_hdr icmp6hdr;
    char buf[1024];

    // create a raw socket for IPv6 packets
    sockfd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // set the source address
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, SRC_IP, &src_addr.sin6_addr);

    // set the destination address
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, DST_IP, &dst_addr.sin6_addr);

    // fill in the IPv6 header
    memset(&iphdr, 0, sizeof(iphdr));
    iphdr.ip6_flow = htonl((6 << 28) | (0 << 20) | 0);
    iphdr.ip6_plen = htons(sizeof(struct icmp6_hdr));
    iphdr.ip6_nxt = IPPROTO_ICMPV6;
    iphdr.ip6_hops = 255;
    iphdr.ip6_src = src_addr.sin6_addr;
    iphdr.ip6_dst = dst_addr.sin6_addr;

    // fill in the ICMPv6 echo request message
    memset(&icmp6hdr, 0, sizeof(icmp6hdr));
    icmp6hdr.icmp6_type = ICMP6_ECHO_REQUEST;
    icmp6hdr.icmp6_code = 0;
    icmp6hdr.icmp6_id = htons(SRC_PORT);
    icmp6hdr.icmp6_seq = htons(1);
    memcpy(buf, &icmp6hdr, sizeof(icmp6hdr));
    memcpy(buf + sizeof(icmp6hdr), MSG, strlen(MSG));

    // send the packet
    rc = sendto(sockfd, buf, sizeof(icmp6hdr) + strlen(MSG), 0, (struct sockaddr *)&dst_addr, sizeof(dst_addr));
    if (rc < 0) {
        perror("sendto");
        exit(1);
    }

    printf("Sent a packet from PCA to PCB\n");

    return 0;
}
