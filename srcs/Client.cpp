/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:00:41 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/01 14:08:48 by mhotting         ###   ########.fr       */
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

void Client::setFd(const int fd) {
    this->_fd = fd;
}

void Client::setIpAddress(const std::string &ipAddress) {
    this->_ipAddress = ipAddress;
}
