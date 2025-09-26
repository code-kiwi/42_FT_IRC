/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:13:10 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 07:03:38 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "CommandFactory.hpp"
#include "IRCReplies.hpp"
#include "config.hpp"
#include "helpers.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <ctime>
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
    while (!this->_commandQueue.empty()) {
        delete this->_commandQueue.front();
        this->_commandQueue.pop();
    }
}

int Server::getSocketFd(void) const {
    return this->_socketFd;
}

int Server::getPort(void) const {
    return this->_port;
}

const std::string &Server::getName(void) const {
    return this->_name;
}

Client *Server::getClientByFd(int fd) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i].getFd() == fd) {
            return &(this->_clients[i]);
        }
    }
    return NULL;
}

void Server::init(void) {
    this->createSocket();
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
                this->sendData(*client);

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

    // Listen for incoming connections
    returned = listen(this->_socketFd, SOMAXCONN);
    if (returned == -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
        throw std::runtime_error("Failed to listen() on the server socket");
    }
    this->_addClientToPollList(this->_socketFd);
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

    client.setFd(incomingFd);
    client.setIpAddress(std::string(ipStr));
    this->_clients.push_back(client);
    this->_addClientToPollList(incomingFd);
#ifdef DEBUG
    std::cout << GREEN << "[DEBUG] Client <" << incomingFd << " - IP " << client.getIpAddress() << "> Connected" << WHITE << std::endl;
#endif
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

void Server::sendData(Client &client) {
    ssize_t n;
    while (!client.getOutputBuffer().empty()) {
        n = send(client.getFd(), client.getOutputBuffer().c_str(), client.getOutputBuffer().size(), MSG_DONTWAIT | MSG_NOSIGNAL);
        if (n > 0) {
            client.consumeOutput(n);
        } else if (n == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                break; // Socket is full, wait until next POLLOUT
            } else {
                clearClient(client.getFd()); // Unexpected error
            }
        }
    }
}

void Server::closeFds(void) {
    // Close all the clients fds
    for (size_t i = 0; i < this->_clients.size(); i++) {
        close(this->_clients[i].getFd());
#ifdef DEBUG
        std::cout << RED << "[DEBUG] Client <" << this->_clients[i].getFd() << "> Disconnected" << WHITE << std::endl;
#endif
        this->_clients[i].setFd(-1);
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

void Server::_addClientToPollList(int fd) {
    struct pollfd newPoll;
    newPoll.fd = fd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    this->_fds.push_back(newPoll);
}

void Server::_extractCommands(void) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        Client &client = this->_clients[i];

        // Getting a vector of raw commands from the client
        std::vector<std::string> rawCommands = client.getRawCommandsFromInputBuffer();
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

void Server::markClientForWrite(int clientFd) {
    for (size_t i = 0; i < this->_fds.size(); i++) {
        if (this->_fds[i].fd == clientFd) {
            this->_fds[i].events |= POLLOUT; // add POLLOUT to the client pollfd
            break;
        }
    }
}

bool Server::isValidPassword(const std::string &password) {
    return password == this->_password;
}

bool Server::isNicknameInUse(const std::string &nick) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i].getNickname() == nick) {
            return true;
        }
    }
    return false;
}

void Server::sendNumericReplyToClient(Client &client, int code, const std::string &message) {
    client.appendToOutputBuffer(
        formatNumericReply(this->_name, code, client.getReplyTarget(), message));
    this->markClientForWrite(client.getFd());
}

void Server::sendNumericReplyToClient(Client &client, int code, const std::string &message, const std::string &param) {
    client.appendToOutputBuffer(
        formatNumericReply(this->_name, code, client.getReplyTarget(), param, message));
    this->markClientForWrite(client.getFd());
}

void Server::sendMessageToClient(Client &client, const Command &command, const std::string &params)
{
    client.appendToOutputBuffer(formatMessage(client.getUsername(), command.getName(), params));
	this->markClientForWrite(client.getFd());
}

void Server::registerClient(Client &client) {
    if (!client.isReadyToRegister()) {
        return;
    }
    client.updateState();

    // Send the welcome message
    sendNumericReplyToClient(client, IRC::RPL_WELCOME, IRC::MSG_WELCOME + " " + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAddress());
    sendNumericReplyToClient(client, IRC::RPL_YOURHOST, IRC::MSG_YOURHOST1 + " " + this->_name + ", " + IRC::MSG_YOURHOST2 + " " + this->_version);
    sendNumericReplyToClient(client, IRC::RPL_CREATED, IRC::MSG_CREATED + " " + this->_creationDate);
    sendNumericReplyToClient(client, IRC::RPL_MYINFO, this->_name + " " + this->_version + " o itkol");

    // Send MOTD
    sendNumericReplyToClient(client, IRC::RPL_MOTDSTART, "- " + this->_name + " " + IRC::MSG_MOTDSTART);
    sendNumericReplyToClient(client, IRC::RPL_MOTD, IRC::MSG_MOTD);
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "   |\\---/|");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "   | ,_, |");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "    \\_`_/-..----.");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, " ___/ `   ' ,''+ \\ ");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "(__...'   __\\    |`.___.';");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "  (_,...'(_,.`__)/'.....+");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "Coded by : lbutel and mhotting");
    sendNumericReplyToClient(client, IRC::RPL_ENDOFMOTD, IRC::MSG_ENDOFMOTD);
}
