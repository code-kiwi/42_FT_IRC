/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:11 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/05 23:36:38 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string InviteCommand::NAME = IRC::CMD_INVITE;

InviteCommand::InviteCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &InviteCommand::getName(void) const {
    return InviteCommand::NAME;
}

void InviteCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] InviteCommand executed for fd " << this->_sender->getFd() << std::endl;
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

    // Get nick and check that it exists
    const std::string &nick = this->_params[0];
    Client *target = server.getClientByNickname(nick);
    if (target == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHNICK, IRC::MSG_NOSUCHNICK, nick);
        return;
    }

    // Get channel and check that it exists
    Channel *channel = server.getChannelByName(this->_params[1]);
    if (channel == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, IRC::MSG_NOSUCHCHANNEL, this->_params[1]);
        return;
    }

    // Check that user is not already on channel
    if (channel->isMember(target)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_USERONCHANNEL, IRC::MSG_NUSERONCHANNEL, nick + " " + channel->getName());
        return;
    }

    // Check if sender has privileges to invite
    if (!channel->isMember(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTONCHANNEL, IRC::MSG_NOTONCHANNEL, channel->getName());
        return;
    }
    if (channel->isInviteOnly() && !channel->isOp(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_CHANOPRIVSNEEDED, IRC::MSG_CHANOPRIVSNEEDED, channel->getName());
        return;
    }

    // Do nothing if already invited
    if (channel->isInvited(target)) {
        return;
    }

    // Add to channel invited list and send messages
    channel->addInvited(target);
    server.sendNumericReplyToClient(this->_sender, IRC::RPL_INVITING, "", target->getNickname() + " " + channel->getName());
    server.sendMessageToClient(this->_sender, target, this->getName() + " " + target->getNickname() + " " + channel->getName(), "");
}