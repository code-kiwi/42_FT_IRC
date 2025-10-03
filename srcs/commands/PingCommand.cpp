/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:17 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:29:47 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PingCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string PingCommand::NAME = IRC::CMD_PING;

PingCommand::PingCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &PingCommand::getName(void) const {
    return PingCommand::NAME;
}

void PingCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] PingCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    (void)server;
}