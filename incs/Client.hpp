/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:00:49 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/30 17:29:00 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Client {
public:
    enum ClientState {
        CONNECTED,
        AUTHENTICATED,
        REGISTERED
    };

    Client(void);
    Client(const int fd, const std::string &ipAddress);
    ~Client(void);

    int getFd(void) const;
    const std::string &getNickname(void) const;
    const std::string &getUsername(void) const;
    const std::string getReplyTarget(void) const;
    const std::string &getIpAddress(void) const;
    const std::string &getInputBuffer(void) const;
    const std::string &getOutputBuffer(void) const;
    ClientState getState(void) const;

    void setFd(const int fd);
    void setIpAddress(const std::string &ipAddress);
    void setNickname(const std::string &nickname);
    void setUser(const std::string &username, const std::string &realname);
    void setPassOk(void);

    void appendToInputBuffer(const std::string &receivedData);
    std::vector<std::string> getRawCommandsFromInputBuffer(void);
    void appendToOutputBuffer(const std::string &str);
    void consumeOutput(size_t n);

    void markForDisconnect(void);
    bool isMarkedForDisconnect(void) const;
    bool isRegistered(void) const;
    bool isReadyToRegister(void) const;
    bool isPassOk(void) const;
    void updateState(void);

private:
    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;

    std::string _ipAddress;
    std::string _inputBuffer;
    std::string _outputBuffer;

    ClientState _state;
    bool _toDisconnect;
    bool _passOk;
    bool _nickSet;
    bool _userSet;
};

#endif
