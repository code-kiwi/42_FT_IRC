/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerGeneral.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:06:06 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 17:36:42 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandFactory.hpp"
#include "Server.hpp"
#include "config.hpp"

#include <ctime>
#include <iostream>
#include <poll.h>
#include <unistd.h>

bool Server::_signalReceived = false;

Server::Server(int port, const std::string &name, const std::string &password, const std::string &version)
    : _socketFd(-1), _port(port), _name(name), _password(password), _version(version) {
    // Save the server creation date
    std::time_t t = std::time(NULL);
    std::tm *tm_ptr = std::localtime(&t);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%a %b %d %Y", tm_ptr);
    _creationDate = buffer;
}

Server::~Server(void) {
    // Remove all clients
    while (!this->_clients.empty()) {
        Client *client = this->_clients.back();
        if (client->getFd() != -1) {
            close(client->getFd());
            client->setFd(-1);
        }
        delete client;
        this->_clients.pop_back();
    }

    // Close main socket
    if (this->_socketFd != -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
    }

    // Remove all commands
    while (!this->_commandQueue.empty()) {
        delete this->_commandQueue.front();
        this->_commandQueue.pop();
    }
}

const std::string &Server::getName(void) const {
    return this->_name;
}

void Server::closeFds(void) {
    // Close all the clients fds
    for (size_t i = 0; i < this->_clients.size(); i++) {
        close(this->_clients[i]->getFd());
#ifdef DEBUG
        std::cout << RED << "[DEBUG] Client <" << this->_clients[i]->getFd() << "> Disconnected" << WHITE << std::endl;
#endif
        this->_clients[i]->setFd(-1);
    }
    this->_fds.clear();

    // Close the server socket
    if (this->_socketFd != -1) {
        close(this->_socketFd);
#ifdef DEBUG
        std::cout << RED << "[DEBUG] Server <" << this->_socketFd << "> Disconnected" << WHITE << std::endl;
#endif
        this->_socketFd = -1;
    }
}

void Server::init(void) {
    this->createSocket();
    this->_printWelcomeMessage();
#ifdef DEBUG
    std::cout << GREEN << "[DEBUG] Server <" << this->_socketFd << "> Connected" << WHITE << std::endl;
    std::cout << "[DEBUG] Waiting for a connection to accept..." << std::endl;
#endif

    // Server's loop until signal handling
    int returned;
    while (!Server::_signalReceived) {
        // Handling client command
        this->_extractCommands();
        this->_processCommands();
#ifdef DEBUG
        std::cout << GREEN << "[DEBUG] Main loop start" << WHITE << std::endl;
        std::cout << YELLOW << "[DEBUG] Server clients (" << this->_clients.size() << ") : [";
        for (size_t i = 0; i < this->_clients.size(); i++) {
            std::cout << this->_clients[i]->getNickname();
            if (i + 1 != this->_clients.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << WHITE << std::endl;
        std::cout << YELLOW << "[DEBUG] Server channels (" << this->_channels.size() << ") : [";
        for (size_t i = 0; i < this->_channels.size(); i++) {
            std::cout << this->_channels[i].getName() << " (users: " << this->_channels[i].getMembers().size() << ")";
            if (i + 1 != this->_channels.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << WHITE << std::endl;
#endif

        // Checking for socket updates
        returned = poll(&(this->_fds[0]), this->_fds.size(), -1);
        if (returned == -1 && !Server::_signalReceived) {
            throw std::runtime_error("The call to poll() failed");
        }

        // Checks all file descriptors revents
        for (size_t i = 0; i < this->_fds.size(); i++) {
            short revents = this->_fds[i].revents;

            // Error detection
            if (revents & (POLLHUP | POLLERR | POLLNVAL)) {
                if (this->_fds[i].fd == this->_socketFd) {
                    throw std::runtime_error("Critical error happened on main server socket");
                } else {
                    clearClient(this->_fds[i].fd);
                    break;
                }
            }

            // Data to read detection
            if (revents & POLLIN) {
                if (this->_fds[i].fd == this->_socketFd) {
                    this->acceptNewClient();
                } else {
                    receiveData(this->_fds[i].fd);
                }
            }

            // Data to send detection
            if (revents & POLLOUT) {
                Client *client = this->getClientByFd(this->_fds[i].fd);
                if (client == NULL) {
                    this->clearClient(this->_fds[i].fd);
                    continue;
                }
                this->sendData(client);

                // No more data to send
                if (client->getOutputBuffer().empty()) {
                    this->_fds[i].events &= ~POLLOUT;
                    if (client->isMarkedForDisconnect()) {
#ifdef DEBUG
                        std::cout << "[DEBUG] Disconnecting client " << (client->getNickname().empty() ? "<no-nick>" : client->getNickname()) << " (fd=" << client->getFd() << ")" << std::endl;
#endif
                        this->clearClient(this->_fds[i].fd);
                        continue;
                    }
                }
            }
        }
    }
    closeFds();
}

void Server::signalHandler(int) {
    Server::_signalReceived = true;
}

bool Server::isValidPassword(const std::string &password) {
    return password == this->_password;
}

void Server::_extractCommands(void) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        Client *client = this->_clients[i];

        // Getting a vector of raw commands from the client
        std::vector<std::string> rawCommands = client->getRawCommandsFromInputBuffer();
        if (rawCommands.empty()) {
            continue;
        }

        // Converting rawCommands into Command objects
        for (size_t i = 0; i < rawCommands.size(); i++) {
            this->_commandQueue.push(CommandFactory::createCommand(rawCommands[i], client));
        }
    }
}

void Server::_processCommands(void) {
    while (!this->_commandQueue.empty()) {
        Command *cmd = this->_commandQueue.front();
        this->_commandQueue.pop();
        if (cmd == NULL) {
            continue;
        }
#ifdef DEBUG
        std::cout << "[DEBUG] Processing command: " << *cmd << std::endl;
#endif
        cmd->execute(*this);
        delete cmd;
    }
}

void Server::_printWelcomeMessage(void) const {
    const std::string displayIp = (this->_serverIp == "::") ? "localhost" : this->_serverIp;
    std::cout << "========================================" << std::endl;
    std::cout << " Welcome to 42_FT_IRC Server" << std::endl;
    std::cout << " Listening on " << displayIp << ":" << this->_port << std::endl;
    std::cout << " Started at " << this->_creationDate << std::endl;
    std::cout << "========================================" << std::endl;
}
