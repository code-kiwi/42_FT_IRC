/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:14:57 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/03 11:42:17 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Client.hpp"
#include "IRCCommands.hpp"

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
    void sendData(Client *client);

    void closeFds(void);
    void clearClient(int fd, const std::string &reason = "");
    void markClientForWrite(int clientFd);

    static void signalHandler(int);

    bool isValidPassword(const std::string &password);
    bool isNicknameInUse(const std::string &nick);

    Channel &addChannel(const std::string &channelName);
    Channel *getChannelByName(const std::string channelName);
    bool isChannelCreated(const std::string channelName);
    void removeChannel(const std::string &channelName);

    void partClientFromAllChannels(Client *client, const std::string &reason, const std::string &commandName = IRC::CMD_PART);
    void partClientFromChannel(Client *client, Channel &channel, const std::string &reason, const std::string &commandName = IRC::CMD_PART);
    void quitClientFromAllChannels(Client *client, const std::string &reason);

    void registerClient(Client *client);

    void sendNumericReplyToClient(Client *client, int code, const std::string &message);
    void sendNumericReplyToClient(Client *client, int code, const std::string &message, const std::string &param);
    void sendMessageToClient(const Client *source_client, Client *dest_client, const std::string &commandName, const std::string &params);
    void sendMessageToChannelUsers(Channel &channel, const Client *source, const std::string &commandName, const std::string &params);
    void sendNamesToClient(Client *client, Channel &channel);

private:
    int _socketFd;
    int _port;
    const std::string _name;
    const std::string _password;
    const std::string _version;
    std::string _creationDate;
    std::vector<Client *> _clients;
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
