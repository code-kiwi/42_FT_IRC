/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:00:41 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/03 20:32:38 by mhotting         ###   ########.fr       */
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

const std::string &Client::getBuffer(void) const {
    return this->_buffer;
}

void Client::setFd(const int fd) {
    this->_fd = fd;
}

void Client::setIpAddress(const std::string &ipAddress) {
    this->_ipAddress = ipAddress;
}

void Client::appendToBuffer(const std::string &receivedData) {
    this->_buffer += receivedData;
}
