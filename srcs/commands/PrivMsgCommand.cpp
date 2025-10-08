/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:19 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/06 03:04:24 by mhotting         ###   ########.fr       */
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
    // Check if user is registered
    if (!this->_sender->isRegistered()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTREGISTERED, IRC::MSG_NOTREGISTERED);
        return;
    }

    // Check if at least 2 params are given
    if (this->_params.size() < 2) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    }

    std::vector<std::string> targets = splitString(this->_params[0], ",");
    const std::string &message = this->_params[1];

    for (size_t i = 0; i < targets.size(); i++) {
        const std::string &targetName = targets[i];

        // Get channel and send message a message on it (if exists)
        Channel *channel = server.getChannelByName(targetName);
        if (channel != NULL) {
            if (!channel->isMember(this->_sender)) {
                server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTONCHANNEL, IRC::MSG_NOTONCHANNEL, channel->getName());
                continue;
            }
            // Send message to all users of the channel (except sender)
            server.sendMessageToChannelUsers(*channel, this->_sender, this->getName() + " " + channel->getName(), message);
            continue;
        }

        // Get user by nick and send message to it (if exists)
        Client *targetClient = server.getClientByNickname(targetName);
        if (targetClient == NULL) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHNICK, IRC::MSG_NOSUCHNICK, targetName);
            continue;
        }
        server.sendMessageToClient(this->_sender, targetClient, this->getName() + " " + targetClient->getNickname(), message);
    }
}