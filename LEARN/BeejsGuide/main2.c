/**
 * TCP server minimal squeleton
 * - prepares an address to listen from
 * - creates a socket
 * - waits for a connection
 * - accepts it
 * NB : Not complete, just here as an example
 * NB : do not forget to add error checking in real cases !
 */

 #include <string.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netdb.h>

 #define MYPORT "3490" // the port users will be connecting to
 #define BACKLOG 10 // how many pending connections queue holds

 int main(void) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;

    // First : load up address structs with addrinfo()
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    getaddrinfo(NULL, MYPORT, &hints, &res);

    // Second : make a socket, bind it and listen on it
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);

    // Third : accept incoming connection
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);

    // Finally : ready to communicate on socket descriptor new_fd !

    return 0;
 }