/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:43:05 by lbutel            #+#    #+#             */
/*   Updated: 2025/10/06 01:06:01 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

#include <sstream>

const std::string JoinCommand::NAME = IRC::CMD_JOIN;

JoinCommand::JoinCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &JoinCommand::getName(void) const {
    return JoinCommand::NAME;
}

void JoinCommand::sendJoinReplies(Server &server, Channel &channel) {
    server.sendMessageToClient(this->_sender, this->_sender, this->getName() + " " + channel.getName(), "");
    server.sendMessageToChannelUsers(channel, this->_sender, this->getName() + " " + channel.getName(), "");
    if (channel.getTopic().empty()) {
        server.sendNumericReplyToClient(this->_sender, IRC::RPL_NOTOPIC, IRC::MSG_NOTOPIC, channel.getName());
    } else {
        server.sendNumericReplyToClient(this->_sender, IRC::RPL_TOPIC, channel.getTopic(), channel.getName());
    }
    server.sendNamesToClient(this->_sender, channel);
}

void JoinCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] JoinCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Check if user is registered
    if (!this->_sender->isRegistered()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTREGISTERED, IRC::MSG_NOTREGISTERED);
        return;
    }

    // Check if params are given
    if (this->_params.empty()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    }

    // JOIN 0 case
    if (this->_params[0] == "0" && this->_params.size() == 1) {
#ifdef DEBUG
        std::cout << "[DEBUG] User " << this->_sender->getNickname() << " asked to quit all its channels" << std::endl;
#endif
        server.partClientFromAllChannels(this->_sender, "");
        return;
    }

    // Get channel information
    std::vector<std::string> channelNames = splitString(this->_params[0], ",");
    std::vector<std::string> channelKeys;
    if (this->_params.size() >= 2)
        channelKeys = splitString(this->_params[1], ",");

    // JOIN execution
    for (size_t i = 0; i < channelNames.size(); i++) {
        std::string &channelName = channelNames[i];
        std::string channelKey = "";
        if (i < channelKeys.size()) {
            channelKey = channelKeys[i];
        }

        // Check channel info
        if (!Channel::isValidName(channelName)) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_BADCHANMASK, IRC::MSG_BADCHANMASK, this->getName());
            continue;
        }
        if (!Channel::isValidKey(channelKey)) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_BADCHANKEY, IRC::MSG_BADCHANKEY1, this->getName());
            continue;
        }

        // Channel already existing
        if (server.isChannelCreated(channelName)) {
            this->joinExistingChannel(server, channelName, channelKey);
            continue;
        }

        // New Channel
        this->joinNewChannel(server, channelName, channelKey);
    }
}

void JoinCommand::joinExistingChannel(Server &server, const std::string &channelName, const std::string &channelKey) {
    Channel *channel = server.getChannelByName(channelName);
    if (channel == NULL) {
        return;
    }
    if (channel->isMember(this->_sender)) {
        return;
    }

    // Check channel limit
    if (channel->hasMemberLimit() && channel->getMemberLimit() > 0 && channel->getMembers().size() >= channel->getMemberLimit()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_CHANNELISFULL, IRC::MSG_CHANNELISFULL, channel->getName());
        return;
    }

    // Handle key
    if (channel->hasKeyMode() && channel->getKey() != channelKey) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_BADCHANKEY, IRC::MSG_BADCHANKEY2, channel->getName());
        return;
    }

    // Handle invite mode
    if (channel->isInviteOnly() && !channel->isInvited(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_INVITEONLYCHAN, IRC::MSG_INVITEONLYCHAN, channel->getName());
        return;
    }
    if (channel->isInvited(this->_sender)) {
        channel->removeInvited(this->_sender);
    }

    channel->addMember(this->_sender);
#ifdef DEBUG
    std::cout << "[DEBUG] JoinCommand adds member " << this->_sender->getNickname() << " to channel " << channel->getName() << std::endl;
#endif
    this->sendJoinReplies(server, *channel);
}

void JoinCommand::joinNewChannel(Server &server, const std::string &channelName, const std::string &channelKey) {
    Channel &newChannel = server.addChannel(channelName);
    newChannel.addMember(this->_sender);
    newChannel.addOp(this->_sender);
#ifdef DEBUG
    std::cout << "[DEBUG] JoinCommand creates the channel " << newChannel.getName() << std::endl;
#endif
    // Add key if given
    if (!channelKey.empty()) {
        newChannel.setKeyMode(true);
        newChannel.setKey(channelKey);
    }
    this->sendJoinReplies(server, newChannel);
}
