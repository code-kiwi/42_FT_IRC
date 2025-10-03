/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:17 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/06 13:23:23 by mhotting         ###   ########.fr       */
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
    if (this->_params.empty()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    };
    const std::string answer = ":" + server.getName() + " PONG " + server.getName() + " :" + this->_params[0] + "\r\n";
    this->_sender->appendToOutputBuffer(answer);
    server.markClientForWrite(this->_sender->getFd());
}