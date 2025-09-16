/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:14:57 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/16 18:34:14 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Command.hpp"

#include <queue>
#include <string>
#include <vector>

class Command;

class Server {
public:
    Server(int port, const std::string &name, const std::string password);
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

private:
    int _socketFd;
    int _port;
    const std::string _name;
    const std::string _password;
    std::vector<Client> _clients;
    std::vector<struct pollfd> _fds;
    static bool _signalReceived;
    std::queue<Command *> _commandQueue;

    Server(void);
    void _addClientToPollList(int fd);

    void _extractCommands(void);
    void _processCommands(void);
};

#endif
