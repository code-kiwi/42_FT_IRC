/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:00:41 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/30 00:22:46 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
    : _fd(-1), _ipAddress(""), _inputBuffer(""), _outputBuffer(""), _state(CONNECTED), _toDisconnect(false), _passOk(false), _nickSet(false), _userSet(false) {}

Client::Client(const int fd, const std::string &ipAddress)
    : _fd(fd), _ipAddress(ipAddress), _inputBuffer(""), _outputBuffer(""), _state(CONNECTED), _toDisconnect(false), _passOk(false), _nickSet(false), _userSet(false) {}

Client::~Client(void) {}

int Client::getFd(void) const {
    return this->_fd;
}

const std::string &Client::getNickname(void) const {
    return this->_nickname;
}

const std::string &Client::getUsername(void) const {
    return this->_username;
}

const std::string &Client::getRealname(void) const {
    return this->_realname;
}

const std::string Client::getReplyTarget(void) const {
    return this->_nickname.empty() ? "*" : this->_nickname;
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

Client::ClientState Client::getState(void) const {
    return this->_state;
}

void Client::setFd(const int fd) {
    this->_fd = fd;
}

void Client::setIpAddress(const std::string &ipAddress) {
    this->_ipAddress = ipAddress;
}

void Client::setNickname(const std::string &nickname) {
    this->_nickname = nickname;
    this->_nickSet = true;
}

void Client::setUser(const std::string &username, const std::string &realname) {
    this->_username = username;
    this->_realname = realname;
    this->_userSet = true;
}

void Client::setPassOk(void) {
    this->_passOk = true;
    this->updateState();
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
        size_t pos = this->_inputBuffer.find("\n");
        if (pos == std::string::npos) {
            break;
        }
        std::string cmd = this->_inputBuffer.substr(0, pos);

        // Remove '\r' at the end of the command line
        if (cmd[cmd.length() - 1] == '\r') {
            cmd = cmd.substr(0, cmd.length() - 1);
        }

        // Add command to the command vector and clean buffer
        rawCommands.push_back(cmd);
        this->_inputBuffer.erase(0, pos + 1);
    }
    return rawCommands;
}

void Client::markForDisconnect(void) {
    this->_toDisconnect = true;
}

bool Client::isMarkedForDisconnect(void) const {
    return this->_toDisconnect;
}

bool Client::isRegistered(void) const {
    return this->_state == REGISTERED;
}

bool Client::isReadyToRegister(void) const {
    return !this->isRegistered() && this->_passOk && this->_nickSet && this->_userSet;
}

bool Client::isPassOk(void) const {
    return this->_passOk;
}

void Client::updateState(void) {
    if (this->_state != REGISTERED && this->isReadyToRegister()) {
        this->_state = REGISTERED;
    } else if (this->_state == CONNECTED && this->_passOk) {
        this->_state = AUTHENTICATED;
    }
}
