/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:27:57 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:30:31 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WhoCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string WhoCommand::NAME = IRC::CMD_WHO;

WhoCommand::WhoCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &WhoCommand::getName(void) const {
    return WhoCommand::NAME;
}

void WhoCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] WhoCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    (void)server;
}