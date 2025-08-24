/**
 * Creates a stream socket server (minimal)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

// Server will listen on port 3490 and 10 connections aan be stored in the queue
#define PORT "3490" // The port users will be connecting to
#define BACKLOG 10 // How many pending connections queue will hold

// Used to clean zombie processes (SIGCHLD handler)
void sigchls_handler(int s) {
    (void) s; // quiet unused variable warning
    int saved_errno = errno; // waitpid() might overwrite errno, we save and restore it
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

/**
 * Get sockaddr, IPv4 or IPv6
 */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

int main(void) {
    // Listen on sock_fd, new connection on new_fd
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // Connector's address info
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int status;

    // Getting adress info
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // Only IPv4
    hints.ai_socktype = SOCK_STREAM; // Using TCP protocol
    hints.ai_flags = AI_PASSIVE; // Use my IP (local address)
    rv = getaddrinfo(NULL, PORT, &hints, &servinfo);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return EXIT_FAILURE;
    }

    // Loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); // Creates a socket and returns its fd (socket does not correspond to anything)
        if (sockfd == -1) {
            perror("Server: socket");
            continue;
        }

        status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (status == -1) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        status = bind(sockfd, p->ai_addr, p->ai_addrlen); // Links the socket to an address
        if (status == -1) {
            close(sockfd);
            perror("Server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
        fprintf(stderr, "Server: failed to bind\n");
        exit(EXIT_FAILURE);
    }

    status = listen(sockfd, BACKLOG);
    if (status == -1) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = sigchls_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    status = sigaction(SIGCHLD, &sa, NULL);
    if (status == -1) {
        perror("Sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Server: waiting for connections...\n");

    while (true) {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);

        status = fork();
        if (status != 0) { // This is the child process
            close(sockfd); // Child does not need the listener
            status = send(new_fd, "Hello, World !", 14, 0);
            if (status == -1) {
                perror("Send");
            }
            close(new_fd);
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
