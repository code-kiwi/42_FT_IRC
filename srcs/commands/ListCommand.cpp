/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:29:51 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 16:51:10 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

#include <sstream>

const std::string ListCommand::NAME = IRC::CMD_LIST;

ListCommand::ListCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &ListCommand::getName(void) const {
    return ListCommand::NAME;
}

void ListCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] ListCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Check if user is registered
    if (!this->_sender->isRegistered()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTREGISTERED, IRC::MSG_NOTREGISTERED);
        return;
    }

    // No param : list all channels
    if (this->_params.size() == 0) {
        const std::vector<Channel> &channels = server.getChannels();
        for (size_t i = 0; i < channels.size(); i++) {
            std::ostringstream oss;
            oss << channels[i].getName() << " " << channels[i].getMemberCount() << " :" << channels[i].getTopic();
            server.sendNumericReplyToClient(this->_sender, IRC::RPL_LIST, "", oss.str());
        }
        server.sendNumericReplyToClient(this->_sender, IRC::RPL_LISTEND, "", IRC::MSG_LISTEND);
        return;
    }

    // Params : handle only given channels
    const std::vector<std::string> channelNames = splitString(this->_params[0], ",");
    for (size_t i = 0; i < channelNames.size(); i++) {
        Channel *channel = server.getChannelByName(channelNames[i]);
        if (channel == NULL) {
            continue;
        }
        std::ostringstream oss;
        oss << channel->getName() << " " << channel->getMemberCount() << " :" << channel->getTopic();
        server.sendNumericReplyToClient(this->_sender, IRC::RPL_LIST, "", oss.str());
    }
    server.sendNumericReplyToClient(this->_sender, IRC::RPL_LISTEND, "", IRC::MSG_LISTEND);
}
