/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:27:57 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 00:06:15 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WhoCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string WhoCommand::NAME = IRC::CMD_WHO;

WhoCommand::WhoCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &WhoCommand::getName(void) const {
    return WhoCommand::NAME;
}

void WhoCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] WhoCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Check if user is registered
    if (!this->_sender->isRegistered()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTREGISTERED, IRC::MSG_NOTREGISTERED);
        return;
    }

    // No param: global user list
    if (this->_params.size() == 0) {
        const std::vector<Client *> clients = server.getAllClients();
        for (size_t i = 0; i < clients.size(); i++) {
            const std::string msg1 = "* " + clients[i]->getUsername() + " " + clients[i]->getIpAddress() + " " + server.getName() + " " + clients[i]->getNickname() + " H";
            const std::string msg2 = "0 " + clients[i]->getRealname();
            server.sendNumericReplyToClient(this->_sender, IRC::RPL_WHOREPLY, msg2, msg1);
        }
        server.sendNumericReplyToClient(this->_sender, IRC::RPL_ENDOFWHO, IRC::MSG_ENDOFWHO, "*");
        return;
    }

    // Channel given as param
    Channel *channel = server.getChannelByName(this->_params[0]);
    if (channel == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, IRC::MSG_NOSUCHCHANNEL, this->_params[0]);
        return;
    }

    // Check that sender is member of channel
    if (!channel->isMember(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTONCHANNEL, IRC::MSG_NOTONCHANNEL, channel->getName());
        return;
    }

    // Send the reply
    std::set<Client *> members = channel->getMembers();
    for (std::set<Client *>::iterator it = members.begin(); it != members.end(); it++) {
        const std::string msg1 = channel->getName() + " " + (*it)->getUsername() + " " + (*it)->getIpAddress() + " " + server.getName() + " " + (*it)->getNickname() + " H";
        const std::string msg2 = "0 " + (*it)->getRealname();
        server.sendNumericReplyToClient(this->_sender, IRC::RPL_WHOREPLY, msg2, msg1);
    }
    server.sendNumericReplyToClient(this->_sender, IRC::RPL_ENDOFWHO, IRC::MSG_ENDOFWHO, channel->getName());
}
