/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:14 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:29:26 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string KickCommand::NAME = IRC::CMD_KICK;

KickCommand::KickCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &KickCommand::getName(void) const {
    return KickCommand::NAME;
}

void KickCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] KickCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    (void)server;
}