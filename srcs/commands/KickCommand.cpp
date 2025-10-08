/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:14 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/06 12:58:51 by mhotting         ###   ########.fr       */
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
    // Check if user is registered
    if (!this->_sender->isRegistered()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTREGISTERED, IRC::MSG_NOTREGISTERED);
        return;
    }

    // Check if at least 2 param are given
    if (this->_params.size() < 2) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    }

    std::vector<std::string> channels = splitString(this->_params[0], ",");
    std::vector<std::string> targets = splitString(this->_params[1], ",");

    // Check one channel or n channels and n corresponding targets
    if (channels.size() > 1 && channels.size() != targets.size()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    }

    // Get comment if it is given
    const std::string comment = this->_params.size() > 2 ? this->_params[2] : "";

    // One channel case
    if (channels.size() == 1) {
        // Get channel and check it
        Channel *channel = this->getAndCheckChannel(server, channels[0]);
        if (channel == NULL) {
            return;
        }

        // KICK all given members
        for (size_t i = 0; i < targets.size(); i++) {
            this->kickMemberFromChannel(server, channel, targets[i], comment);
        }

        // Remove channel if empty
        if (channel->getMemberCount() == 0) {
            server.removeChannel(channel->getName());
        }

        return;
    }

    // Multiple channels, each with exactly 1 user
    for (size_t i = 0; i < channels.size(); i++) {
        // Get channel
        Channel *channel = this->getAndCheckChannel(server, channels[i]);
        if (channel == NULL) {
            continue;
        }

        // KICK the corresponding member
        this->kickMemberFromChannel(server, channel, targets[i], comment);

        // Remove channel if empty
        if (channel->getMemberCount() == 0) {
            server.removeChannel(channel->getName());
        }
    }
}

void KickCommand::kickMemberFromChannel(Server &server, Channel *channel, const std::string &targetName, const std::string &comment) {
    Client *targetClient = server.getClientByNickname(targetName);
    if (targetClient == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHNICK, IRC::MSG_NOSUCHNICK, targetName);
        return;
    }
    if (!channel->isMember(targetClient)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_USERNOTINCHANNEL, IRC::MSG_USERNOTINCHANNEL, targetName + " " + channel->getName());
        return;
    }
    const std::string msg = this->getName() + " " + channel->getName() + " " + targetClient->getNickname();
    server.sendMessageToClient(this->_sender, this->_sender, msg, comment);
    server.sendMessageToChannelUsers(*channel, this->_sender, msg, comment);
    channel->kickMember(targetClient);
}

Channel *KickCommand::getAndCheckChannel(Server &server, const std::string &channelName) {
    // Get channel
    Channel *channel = server.getChannelByName(channelName);
    if (channel == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, IRC::MSG_NOSUCHCHANNEL, channelName);
        return NULL;
    }

    // Check that sender is authorized to KICK
    if (!channel->isMember(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTONCHANNEL, IRC::MSG_NOTONCHANNEL, channel->getName());
        return NULL;
    }
    if (!channel->isOp(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_CHANOPRIVSNEEDED, IRC::MSG_CHANOPRIVSNEEDED, channel->getName());
        return NULL;
    }
    return channel;
}
