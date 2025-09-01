/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:13:10 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/01 20:35:48 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "config.hpp"

#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

bool Server::_signalReceived = false;

void Server::signalHandler(int) {
    Server::_signalReceived = true;
}

Server::Server(int port, const std::string password) : _socketFd(-1), _port(port), _password(password) {}

Server::~Server(void) {}

int Server::getSocketFd(void) const {
    return this->_socketFd;
}

int Server::getPort(void) const {
    return this->_port;
}

void Server::init(void) {
    this->createSocket();
#ifdef DEBUG
    std::cout << GREEN << "Server <" << this->_socketFd << "> Connected" << WHITE << std::endl;
    std::cout << "Waiting to accept a connection..." << std::endl;
#endif

    // Server's loop until signal handling
    int returned;
    while (!Server::_signalReceived) {
        returned = poll(&(this->_fds[0]), this->_fds.size(), -1);
        if (returned == -1 && !Server::_signalReceived) {
            throw std::runtime_error("The call to poll() failed");
        }

        // Checks all file descriptors
        for (size_t i = 0; i < this->_fds.size(); i++) {
            // Checks if there is data to read
            if (this->_fds[i].revents & POLLIN) {
                if (this->_fds[i].fd == this->_socketFd) {
                    this->acceptNewClient();
                } else {
                    receiveData(this->_fds[i].fd);
                }
            }
        }
    }
    closeFds();
}

void Server::createSocket(void) {
    int returned = 0;

    // Socket creation
    this->_socketFd = socket(AF_INET6, SOCK_STREAM, 0);
    if (this->_socketFd == -1) { // Checks if the socket creation worked
        throw std::runtime_error("Failed to create the server socket");
    }

    // Set the socket option SO_REUSEADDR to reuse the address (avoids the "address already in use error with bind")
    int en = 1;
    returned = setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en));
    if (returned == -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
        throw std::runtime_error("Failed to set option SO_REUSEADDR to the server socket");
    }

    // Sets the option IPV6_V6ONLY on the socket for accepting IPv4 and IPv6 connections
    int no = 0;
    returned = setsockopt(this->_socketFd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no));
    if (returned == -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
        throw std::runtime_error("Failed to set option IPV6_V6ONLY to the server socket");
    }

    // Set the socket option O_NONBLOCK  for non-blocking socket (even blocking functions will not block the socket, an error will be returned instead)
    returned = fcntl(this->_socketFd, F_SETFL, O_NONBLOCK);
    if (returned == -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
        throw std::runtime_error("Failed to set option O_NONBLOCK on the server socket");
    }

    // Bind the socket to the address
    struct sockaddr_in6 serverAddress;
    const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
    serverAddress.sin6_family = AF_INET6;
    serverAddress.sin6_port = htons(this->_port);
    serverAddress.sin6_addr = in6addr_any;

    returned = bind(this->_socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (returned == -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
        throw std::runtime_error("Failed to bind the server socket");
    }

    // Listen for incoming connections
    returned = listen(this->_socketFd, SOMAXCONN);
    if (returned == -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
        throw std::runtime_error("Failed to listen() on the server socket");
    }
    this->addClientToPoll(this->_socketFd);
}

void Server::acceptNewClient(void) {
    Client client;
    struct sockaddr_storage clientAddress;
    socklen_t len = sizeof(clientAddress);
    int returned = 0;

    // Accept new client
    int incomingFd = accept(this->_socketFd, (struct sockaddr *)&(clientAddress), &len);
    if (incomingFd == -1) {
        if (errno == EINTR) {
#ifdef DEBUG
            std::cout << "accept() interrupted, retrying later" << std::endl;
#endif
            return;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
#ifdef DEBUG
            std::cout << "accept() would block, retrying later" << std::endl;
#endif
            return;
        }
        throw std::runtime_error(std::string("accept() failed: ") + strerror(errno));
    }

    // Sets the socket option O_NONBLOCK for non-blocking socket
    returned = fcntl(incomingFd, F_SETFL, O_NONBLOCK);
    if (returned == -1) {
        close(incomingFd);
        throw std::runtime_error(std::string("fcntl() failed into server::acceptClient: ") + strerror(errno));
    }

    // Detects the IP address' type and converts it to a string
    char ipStr[INET6_ADDRSTRLEN];
    if (clientAddress.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&clientAddress;
        inet_ntop(AF_INET, &(s->sin_addr), ipStr, sizeof(ipStr));
    } else {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&clientAddress;
        inet_ntop(AF_INET6, &(s->sin6_addr), ipStr, sizeof(ipStr));
    }

    client.setFd(incomingFd);
    client.setIpAddress(std::string(ipStr));
    this->_clients.push_back(client);
    this->addClientToPoll(incomingFd);

#ifdef DEBUG
    std::cout << GREEN << "Client <" << incomingFd << " - IP : " << client.getIpAddress() << "> Connected" << WHITE << std::endl;
#endif
}

void Server::receiveData(int fd) {
    char buffer[1024];                 // buffer for received data
    memset(buffer, 0, sizeof(buffer)); // Clears the buffer

    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0); // Receive the data

    if (bytes <= 0) {
        // Client disconnection
        std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
        this->clearClient(fd); // Removes the client from saved ones
    } else {
        // Prints the received data
        buffer[bytes] = '\0';
        std::cout << YELLOW << "Client <" << fd << "> Data: " << WHITE << buffer << std::endl;
    }
}

void Server::closeFds(void) {
    // Close all the clients fds
    for (size_t i = 0; i < this->_clients.size(); i++) {
        close(this->_clients[i].getFd());
#ifdef DEBUG
        std::cout << RED << "Client <" << this->_clients[i].getFd() << "> Disconnected" << WHITE << std::endl;
#endif
        this->_clients[i].setFd(-1);
    }
    this->_fds.clear();

    // Close the server socket
    if (this->_socketFd != -1) {
        close(this->_socketFd);
#ifdef DEBUG
        std::cout << RED << "Server <" << this->_socketFd << "> Disconnected" << WHITE << std::endl;
#endif
        this->_socketFd = -1;
    }
}

void Server::clearClient(int fd) {
    // Remove the client from the pollfds
    for (size_t i = 0; i < this->_fds.size(); i++) {
        if (this->_fds[i].fd == fd) {
            this->_fds.erase(this->_fds.begin() + i);
            break;
        }
    }
    // Remove the client from the client's vector
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i].getFd() == fd) {
            this->_clients.erase(this->_clients.begin() + i);
            break;
        }
    }
    // Close the client's fd
    close(fd);
}

void Server::addClientToPoll(int fd) {
    struct pollfd newPoll;
    newPoll.fd = fd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    this->_fds.push_back(newPoll);
}
