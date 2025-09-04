/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:00:41 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/04 19:27:16 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) {}

Client::Client(const int fd, const std::string &ipAddress) : _fd(fd), _ipAddress(ipAddress) {}

Client::~Client(void) {}

int Client::getFd(void) const {
    return this->_fd;
}

const std::string &Client::getIpAddress(void) const {
    return this->_ipAddress;
}

const std::string &Client::getInputBuffer(void) const {
    return this->_inputBuffer;
}

void Client::setFd(const int fd) {
    this->_fd = fd;
}

void Client::setIpAddress(const std::string &ipAddress) {
    this->_ipAddress = ipAddress;
}

void Client::appendToInputBuffer(const std::string &receivedData) {
    this->_inputBuffer += receivedData;
}

std::vector<std::string> Client::getRawCommandsFromInputBuffer(void) {
    std::vector<std::string> rawCommands;

    while (true) {
        size_t pos = this->_inputBuffer.find("\r\n");
        if (pos == std::string::npos) {
            break;
        }
        rawCommands.push_back(this->_inputBuffer.substr(0, pos));
        this->_inputBuffer.erase(0, pos + 2);
    }
    return rawCommands;
}
