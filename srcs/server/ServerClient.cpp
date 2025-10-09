/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:06:06 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 17:37:55 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "config.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <poll.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

Client *Server::getClientByFd(int fd) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i]->getFd() == fd) {
            return this->_clients[i];
        }
    }
    return NULL;
}

Client *Server::getClientByNickname(const std::string &nick) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i]->getNickname() == nick) {
            return this->_clients[i];
        }
    }
    return NULL;
}

const std::vector<Client *> &Server::getAllClients(void) const {
    return this->_clients;
}

void Server::acceptNewClient(void) {
    Client *client = new Client();
    struct sockaddr_storage clientAddress;
    socklen_t len = sizeof(clientAddress);
    int returned = 0;

    // Accept new client
    int incomingFd = accept(this->_socketFd, (struct sockaddr *)&(clientAddress), &len);
    if (incomingFd == -1) {
        if (errno == EINTR) {
#ifdef DEBUG
            std::cout << "[DEBUG] accept() interrupted, retrying later" << std::endl;
#endif
            return;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
#ifdef DEBUG
            std::cout << "[DEBUG] accept() would block, retrying later" << std::endl;
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

    // Configure client (clean IP)
    client->setFd(incomingFd);
    std::string ipAdrr = std::string(ipStr);
    if (ipAdrr.find("::ffff:") == 0) {
        ipAdrr = ipAdrr.substr(7);
    }
    client->setIpAddress(ipAdrr);
    this->_clients.push_back(client);
    this->_addClientToPollList(incomingFd);
#ifdef DEBUG
    std::cout << GREEN << "[DEBUG] Client <" << incomingFd << " - IP " << client->getIpAddress() << "> Connected" << WHITE << std::endl;
#endif
}

void Server::clearClient(int fd, const std::string &reason) {
    // Remove the client from the channels
    Client *client = this->getClientByFd(fd);
    if (client != NULL) {
        this->quitClientFromAllChannels(client, reason);
    }

    // Remove the client from the pollfds
    for (size_t i = 0; i < this->_fds.size(); i++) {
        if (this->_fds[i].fd == fd) {
            this->_fds.erase(this->_fds.begin() + i);
            break;
        }
    }
    // Remove the client from the client's vector
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i]->getFd() == fd) {
            if (fd != -1) {
                close(fd);
            }
            delete this->_clients[i];
            this->_clients.erase(this->_clients.begin() + i);
            break;
        }
    }
}

void Server::markClientForWrite(int clientFd) {
    for (size_t i = 0; i < this->_fds.size(); i++) {
        if (this->_fds[i].fd == clientFd) {
            this->_fds[i].events |= POLLOUT; // add POLLOUT to the client pollfd
            break;
        }
    }
}

bool Server::isNicknameInUse(const std::string &nick) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i]->getNickname() == nick) {
            return true;
        }
    }
    return false;
}

void Server::_addClientToPollList(int fd) {
    struct pollfd newPoll;
    newPoll.fd = fd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    this->_fds.push_back(newPoll);
}
