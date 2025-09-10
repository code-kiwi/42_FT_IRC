/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:00:49 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/01 12:22:12 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
    Client(void);
    Client(const int fd, const std::string &ipAddress);
    ~Client(void);

    int getFd(void) const;
    const std::string &getIpAddress(void) const;

    void setFd(const int fd);
    void setIpAddress(const std::string &ipAddress);

private:
    int _fd;
    std::string _ipAddress;
};

#endif
