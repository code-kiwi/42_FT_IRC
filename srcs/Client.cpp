/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:00:41 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/16 18:33:10 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) {}

Client::Client(const int fd, const std::string &ipAddress)
    : _fd(fd), _ipAddress(ipAddress) {}

Client::~Client(void) {}

int Client::getFd(void) const {
    return this->_fd;
}

const std::string &Client::getNickname(void) const {
    return this->_nickname;
}

const std::string &Client::getIpAddress(void) const {
    return this->_ipAddress;
}

const std::string &Client::getInputBuffer(void) const {
    return this->_inputBuffer;
}

const std::string &Client::getOutputBuffer(void) const {
    return this->_outputBuffer;
}

void Client::setFd(const int fd) {
    this->_fd = fd;
}

void Client::setIpAddress(const std::string &ipAddress) {
    this->_ipAddress = ipAddress;
}

void Client::setNickname(const std::string &nickname) {
    this->_nickname = nickname;
}

void Client::appendToInputBuffer(const std::string &receivedData) {
    this->_inputBuffer += receivedData;
}

void Client::appendToOutputBuffer(const std::string &str) {
    this->_outputBuffer += str;
}

void Client::consumeOutput(size_t n) {
    if (n >= this->_outputBuffer.size()) {
        this->_outputBuffer.clear();
    } else {
        this->_outputBuffer.erase(0, n);
    }
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
