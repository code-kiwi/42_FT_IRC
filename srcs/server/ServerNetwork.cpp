/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNetwork.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:06:06 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 17:36:58 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "config.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

int Server::getSocketFd(void) const {
    return this->_socketFd;
}

int Server::getPort(void) const {
    return this->_port;
}

void Server::createSocket(void) {
    int returned = 0;

    // Socket creation
    this->_socketFd = socket(AF_INET6, SOCK_STREAM, 0);
    if (this->_socketFd == -1) {
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
    memset(&serverAddress, 0, sizeof(serverAddress));
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

    // Get server IP address
    struct sockaddr_in6 addr;
    socklen_t len = sizeof(addr);
    if (getsockname(this->_socketFd, (struct sockaddr *)&addr, &len) == -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
        throw std::runtime_error("Cannot get server socket address");
    }
    char ipStr[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &addr.sin6_addr, ipStr, sizeof(ipStr));
    this->_serverIp = std::string(ipStr);

    // Listen for incoming connections
    returned = listen(this->_socketFd, SOMAXCONN);
    if (returned == -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
        throw std::runtime_error("Failed to listen() on the server socket");
    }
    this->_addClientToPollList(this->_socketFd);
}

void Server::receiveData(int fd) {
    std::string buffer(BUFFER_SIZE, '\0');

    // Getting a pointer to the client linked to fd
    Client *client = this->getClientByFd(fd);
    if (client == NULL) {
        while (recv(fd, &buffer[0], buffer.size(), 0) > 0) {}
#ifdef DEBUG
        std::cerr << "[DEBUG] Warning: received data for unknown fd " << fd << std::endl;
#endif
        return;
    }

    // Reading from the socket
    ssize_t bytes;
    do {
        bytes = recv(fd, &buffer[0], buffer.size(), 0);
        if (bytes > 0) {
#ifdef DEBUG
            std::cout << YELLOW << "[DEBUG] Client <" << client->getFd() << "> Data: " << WHITE << std::string(buffer, 0, bytes) << std::endl;
#endif
            client->appendToInputBuffer(std::string(buffer, 0, bytes));
        }
    } while (bytes > 0);

    // Checking socket shutdown or reading error
    if (bytes == 0) {
#ifdef DEBUG
        std::cout << GREEN << "[DEBUG] Client <" << client->getFd() << "> Disconnected" << WHITE << std::endl;
#endif
        this->clearClient(client->getFd());
    } else if (bytes == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {
#ifdef DEBUG
        std::cerr << "[DEBUG] Error: recv() failed for fd " << client->getFd();
#endif
        clearClient(client->getFd());
    }
}

void Server::sendData(Client *client) {
    ssize_t n;
    while (!client->getOutputBuffer().empty()) {
        n = send(client->getFd(), client->getOutputBuffer().c_str(), client->getOutputBuffer().size(), MSG_DONTWAIT | MSG_NOSIGNAL);
        if (n > 0) {
            client->consumeOutput(n);
        } else if (n == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                break; // Socket is full, wait until next POLLOUT
            } else {
                clearClient(client->getFd()); // Unexpected error
            }
        }
    }
}
