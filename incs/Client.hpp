/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:00:49 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/04 19:27:19 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Client {
public:
    Client(void);
    Client(const int fd, const std::string &ipAddress);
    ~Client(void);

    int getFd(void) const;
    const std::string &getIpAddress(void) const;
    const std::string &getInputBuffer(void) const;

    void setFd(const int fd);
    void setIpAddress(const std::string &ipAddress);

    void appendToInputBuffer(const std::string &receivedData);
    std::vector<std::string> getRawCommandsFromInputBuffer(void);

private:
    int _fd;
    std::string _ipAddress;
    std::string _inputBuffer;
};

#endif
