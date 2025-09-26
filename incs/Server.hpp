/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:14:57 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 05:04:25 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"

#include <queue>
#include <string>
#include <vector>

class Command;

class Server {
public:
    Server(int port, const std::string &name, const std::string &password, const std::string &version);
    ~Server(void);

    int getSocketFd(void) const;
    int getPort(void) const;
    const std::string &getName(void) const;
    Client *getClientByFd(int fd);

    void init(void);
    void createSocket(void);
    void acceptNewClient(void);
    void receiveData(int fd);
    void sendData(Client &client);

    void closeFds(void);
    void clearClient(int fd);
    void markClientForWrite(int clientFd);

    static void signalHandler(int);

    bool isValidPassword(const std::string &password);
    bool isNicknameInUse(const std::string &nick);

	bool isChannelCreated(const std::string channel_name);

    void registerClient(Client &client);
    void sendNumericReplyToClient(Client &client, int code, const std::string &message);
    void sendNumericReplyToClient(Client &client, int code, const std::string &message, const std::string &param);
    void sendMessageToClient(Client &client, const Command &command, const std::string &message);

private:
    int _socketFd;
    int _port;
    const std::string _name;
    const std::string _password;
    const std::string _version;
    std::string _creationDate;
    std::vector<Client> _clients;
    std::vector<Channel> _channels;
    std::vector<struct pollfd> _fds;
    static bool _signalReceived;
    std::queue<Command *> _commandQueue;

    Server(void);
    void _addClientToPollList(int fd);

    void _extractCommands(void);
    void _processCommands(void);
};

#endif
