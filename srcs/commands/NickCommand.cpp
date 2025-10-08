/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:49:10 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/06 23:34:33 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string NickCommand::NAME = IRC::CMD_NICK;

NickCommand::NickCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &NickCommand::getName(void) const {
    return NickCommand::NAME;
}

void NickCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] NickCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Check if at least one param
    if (this->_params.empty()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NONICKNAMEGIVEN, IRC::MSG_NONICKNAMEGIVEN);
        return;
    }

    // Check if PASS was sent
    if (!this->_sender->isPassOk()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_PASSWDMISMATCH, IRC::MSG_PASSWDMISMATCH);
        return;
    }

    const std::string &nick = this->_params[0];
    // Check if nick is valid
    if (!isValidNickname(nick)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_ERRONEUSNICKNAME, IRC::MSG_ERRONEUSNICKNAME, nick);
        return;
    }

    // Check if nickname is available
    if (server.isNicknameInUse(nick)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NICKNAMEINUSE, IRC::MSG_NICKNAMEINUSE, nick);
        return;
    }

    // Client registration
    if (!this->_sender->isRegistered()) {
        this->_sender->setNickname(nick);
        if (this->_sender->isReadyToRegister()) {
            server.registerClient(this->_sender);
        }
        return;
    }

    // Client nick change and broadcast message
    std::set<Client *> peers = server.getChannelPeers(this->_sender);
    for (std::set<Client *>::iterator it = peers.begin(); it != peers.end(); it++) {
        server.sendMessageToClient(this->_sender, *it, this->getName(), nick);
    }
    server.sendMessageToClient(this->_sender, this->_sender, this->getName(), nick);
    this->_sender->setNickname(nick);
}
