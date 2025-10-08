/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 06:22:05 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 12:17:57 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"

const std::string UserCommand::NAME = IRC::CMD_USER;

UserCommand::UserCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &UserCommand::getName(void) const {
    return UserCommand::NAME;
}

void UserCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] UserCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Check if enough parameters
    if (this->_params.size() < 4) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS);
        return;
    }

    // Check if PASS was sent
    if (!this->_sender->isPassOk()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_PASSWDMISMATCH, IRC::MSG_PASSWDMISMATCH);
        return;
    }

    // Save info after a clean
    std::string &username = this->_params[0];
    std::string &realname = this->_params[3];

    // Control username
    if (username.length() == 0 || std::isdigit(username[0])) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_ERRONEUSNICKNAME, IRC::MSG_ERRONEUSNICKNAME2, username);
        return;
    }

    // Control realname
    if (realname.length() == 0 || std::isdigit(realname[0])) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_ERRONEUSNICKNAME, IRC::MSG_ERRONEUSNICKNAME3, realname);
        return;
    }

    // Handle username len
    if (username.size() > 9)
        username = username.substr(0, 9);

    // Handle realname len
    if (realname.size() > 50) {
        realname = realname.substr(0, 50);
    }

    // Set user info
    this->_sender->setUser(username, realname);

    // Register the client if ready
    if (this->_sender->isReadyToRegister()) {
        server.registerClient(this->_sender);
    }
}
