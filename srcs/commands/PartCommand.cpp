/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 03:14:36 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/03 03:34:48 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCommand.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string PartCommand::NAME = IRC::CMD_PART;

PartCommand::PartCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &PartCommand::getName(void) const {
    return PartCommand::NAME;
}

void PartCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] PartCommand executed for fd " << this->_sender->getFd() << std::endl;
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

    // Get Part information
    std::vector<std::string> channelNames = splitString(this->_params[0], ",");
    const std::string reason = this->_params.size() > 1 ? this->_params[1] : "";

    // PART execution
    for (size_t i = 0; i < channelNames.size(); i++) {
        Channel *channel = server.getChannelByName(channelNames[i]);

        // Channel does not exist
        if (channel == NULL) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, IRC::MSG_NOSUCHCHANNEL, channelNames[i]);
            continue;
        }

        // Client is not member of channel
        if (!channel->isMember(this->_sender)) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTONCHANNEL, IRC::MSG_NOTONCHANNEL, channelNames[i]);
            continue;
        }

        // Remove member from channel
        server.partClientFromChannel(this->_sender, *channel, reason);
    }
}
