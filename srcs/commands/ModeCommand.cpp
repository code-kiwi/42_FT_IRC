/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:20:45 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/03 12:23:12 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string ModeCommand::NAME = IRC::CMD_MODE;

ModeCommand::ModeCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &ModeCommand::getName(void) const {
    return ModeCommand::NAME;
}

void ModeCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] ModeCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    (void)server;
}
