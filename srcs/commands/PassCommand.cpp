/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:30:20 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 04:55:23 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"

const std::string PassCommand::NAME = IRC::CMD_PASS;

PassCommand::PassCommand(Client &sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &PassCommand::getName(void) const {
    return PassCommand::NAME;
}

void PassCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] PassCommand executed for fd " << this->_sender.getFd() << std::endl;
#endif
    // Client already approved
    if (this->_sender.getState() != Client::CONNECTED) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_ALREADYREGISTERED, IRC::MSG_ALREADYREGISTERED);
        return;
    }

    // Check if at least one param
    if (this->_params.empty()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    }

    // Checking if password is valid
    const std::string &password = this->_params[0];
    if (server.isValidPassword(password)) {
        this->_sender.setPassOk();
    } else {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_PASSWDMISMATCH, IRC::MSG_PASSWDMISMATCH);
        this->_sender.markForDisconnect();
    }
}
