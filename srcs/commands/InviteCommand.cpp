/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:11 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:28:59 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string InviteCommand::NAME = IRC::CMD_INVITE;

InviteCommand::InviteCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &InviteCommand::getName(void) const {
    return InviteCommand::NAME;
}

void InviteCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] InviteCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    (void)server;
}