/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:15:42 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 04:54:36 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Client.hpp"
#include "Server.hpp"

#include <iostream>
#include <vector>

class Server;

class Command {
public:
    Command(Client &client);
    Command(Client &client, const std::vector<std::string> &params);
    virtual ~Command(void);
    virtual void execute(Server &server) = 0;
    virtual const std::string &getName() const = 0;
    const std::vector<std::string> &getParams(void) const;
    const Client &getSender(void) const;

protected:
    Client &_sender;
    std::vector<std::string> _params;
};

std::ostream &operator<<(std::ostream &o, const Command &command);

#endif
