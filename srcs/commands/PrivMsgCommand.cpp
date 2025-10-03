/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:19 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:30:02 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivMsgCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string PrivMsgCommand::NAME = IRC::CMD_PRIVMSG;

PrivMsgCommand::PrivMsgCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &PrivMsgCommand::getName(void) const {
    return PrivMsgCommand::NAME;
}

void PrivMsgCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] PrivMsgCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    (void)server;
}