/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NamesCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:31:08 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/07 23:01:15 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NamesCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string NamesCommand::NAME = IRC::CMD_NAMES;

NamesCommand::NamesCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &NamesCommand::getName(void) const {
    return NamesCommand::NAME;
}

void NamesCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] NamesCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Check if user is registered
    if (!this->_sender->isRegistered()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTREGISTERED, IRC::MSG_NOTREGISTERED);
        return;
    }

    // Get all the channels to handle
    std::vector<std::string> channelNames;
    if (this->_params.empty()) {
        channelNames = server.getAllChannelNames();
    } else {
        channelNames = splitString(this->_params[0], ",");
    }

    // Send list for each channel listed
    for (size_t i = 0; i < channelNames.size(); i++) {
        Channel *channel = server.getChannelByName(channelNames[i]);
        if (channel == NULL) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, IRC::MSG_NOSUCHCHANNEL, channelNames[i]);
            continue;
        }
        server.sendNamesToClient(this->_sender, *channel);
    }
}