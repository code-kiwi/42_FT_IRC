/**
 * Shows IP addresses for a host given on the command line
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netdb.h>
 #include <arpa/inet.h>
 #include <netinet/in.h>

 int main(int argc, char **argv) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "usage: showip hostname\n");
        return EXIT_FAILURE;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPv6 or IPv4
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(argv[1], NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return (EXIT_FAILURE);
    }

    printf("IP addresses for %s: \n\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;
        struct sockaddr_in *ipv4;
        struct sockaddr_in6 *ipv6;

        // Get the pointer to the address itself
        // Different fields in IPv4 and IPv6
        if (p->ai_family == AF_INET) { // IPv4
            ipv4 = (struct sockaddr_in *) p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Convert the IP to a string and print it
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res); // Free the linked list

    return 0;
 }