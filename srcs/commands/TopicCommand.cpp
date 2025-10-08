/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:22 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/06 00:06:27 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string TopicCommand::NAME = IRC::CMD_TOPIC;

TopicCommand::TopicCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &TopicCommand::getName(void) const {
    return TopicCommand::NAME;
}

void TopicCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] TopicCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Check if user is registered
    if (!this->_sender->isRegistered()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTREGISTERED, IRC::MSG_NOTREGISTERED);
        return;
    }

    // Check if at least 1 param
    if (this->_params.size() == 0) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    }

    // Get channel and check that it exists
    Channel *channel = server.getChannelByName(this->_params[0]);
    if (channel == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, IRC::MSG_NOSUCHCHANNEL, this->_params[0]);
        return;
    }

    // Check that sender is member of the channel
    if (!channel->isMember(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTONCHANNEL, IRC::MSG_NOTONCHANNEL, channel->getName());
        return;
    }

    // User only ask for current topic
    if (this->_params.size() == 1) {
        if (channel->getTopic().empty()) {
            server.sendNumericReplyToClient(this->_sender, IRC::RPL_NOTOPIC, IRC::MSG_NOTOPIC, channel->getName());
        } else {
            server.sendNumericReplyToClient(this->_sender, IRC::RPL_TOPIC, channel->getTopic(), channel->getName());
        }
        return;
    }

    // Check +o topic mode and user op status
    if (channel->isTopicRestricted() && !channel->isOp(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_CHANOPRIVSNEEDED, IRC::MSG_CHANOPRIVSNEEDED, channel->getName());
        return;
    }

    // Get topic and ignore it if too long
    const std::string &newTopic = this->_params[1];
    if (newTopic.length() > 200) {
        return;
    }

    channel->setTopic(newTopic);
    server.sendMessageToClient(this->_sender, this->_sender, this->getName() + " " + channel->getName(), newTopic);
    server.sendMessageToChannelUsers(*channel, this->_sender, this->getName() + " " + channel->getName(), newTopic);
}