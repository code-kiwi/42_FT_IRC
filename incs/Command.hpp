/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:15:42 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/05 18:25:24 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Client.hpp"
#include "Server.hpp"

class Server;

class Command {
public:
    Command(Client &client);
    virtual ~Command(void);
    virtual void execute(Server &server) = 0;

protected:
    Client &_sender;
};

#endif
