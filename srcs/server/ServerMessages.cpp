/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessages.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:06:06 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 18:34:26 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCReplies.hpp"
#include "Server.hpp"
#include "helpers.hpp"

#include <sstream>

void Server::registerClient(Client *client) {
    if (!client->isReadyToRegister()) {
        return;
    }
    client->updateState();

    // Send the welcome message
    sendNumericReplyToClient(client, IRC::RPL_WELCOME, IRC::MSG_WELCOME + " " + client->getNickname() + "!" + client->getUsername() + "@" + client->getIpAddress());
    sendNumericReplyToClient(client, IRC::RPL_YOURHOST, IRC::MSG_YOURHOST1 + " " + this->_name + ", " + IRC::MSG_YOURHOST2 + " " + this->_version);
    sendNumericReplyToClient(client, IRC::RPL_CREATED, IRC::MSG_CREATED + " " + this->_creationDate);
    sendNumericReplyToClient(client, IRC::RPL_MYINFO, this->_name + " " + this->_version + " o itkol");

    // Send MOTD
    sendNumericReplyToClient(client, IRC::RPL_MOTDSTART, "- " + this->_name + " " + IRC::MSG_MOTDSTART);
    sendNumericReplyToClient(client, IRC::RPL_MOTD, IRC::MSG_MOTD);
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "   |\\---/|");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "   | ,_, |");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "    \\_`_/-..----.");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, " ___/ `   ' ,''+ \\ ");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "(__...'   __\\    |`.___.';");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "  (_,...'(_,.`__)/'.....+");
    sendNumericReplyToClient(client, IRC::RPL_MOTD, "Coded by : lbutel and mhotting");
    sendNumericReplyToClient(client, IRC::RPL_ENDOFMOTD, IRC::MSG_ENDOFMOTD);
}

void Server::sendNumericReplyToClient(Client *client, int code, const std::string &message) {
    client->appendToOutputBuffer(
        formatNumericReply(this->_name, code, client->getReplyTarget(), message));
    this->markClientForWrite(client->getFd());
}

void Server::sendNumericReplyToClient(Client *client, int code, const std::string &message, const std::string &param) {
    client->appendToOutputBuffer(formatNumericReply(this->_name, code, client->getReplyTarget(), param, message));
    this->markClientForWrite(client->getFd());
}

void Server::sendMessageToClient(const Client *source_client, Client *dest_client, const std::string &commandName, const std::string &params) {
    dest_client->appendToOutputBuffer(formatMessage(source_client, commandName, params));
    this->markClientForWrite(dest_client->getFd());
}

void Server::sendMessageToClient(Client *dest_client, const std::string &commandName, const std::string &params) {
    dest_client->appendToOutputBuffer(formatMessage(this->getName(), commandName, params));
    this->markClientForWrite(dest_client->getFd());
}

void Server::sendMessageToChannelUsers(Channel &channel, const Client *source, const std::string &commandName, const std::string &params) {
    std::set<Client *> &members = channel.getMembers();
    for (std::set<Client *>::iterator it = members.begin(); it != members.end(); it++) {
        if (*it != source) {
            this->sendMessageToClient(source, *it, commandName, params);
        }
    }
}

void Server::sendMessageToChannelUsers(Channel &channel, const std::string &commandName, const std::string &params) {
    std::set<Client *> &members = channel.getMembers();
    for (std::set<Client *>::iterator it = members.begin(); it != members.end(); it++) {
        this->sendMessageToClient(*it, commandName, params);
    }
}

void Server::sendNamesToClient(Client *client, Channel &channel) {
    std::ostringstream params;
    std::ostringstream message;

    params << "= " << channel.getName();
    std::set<Client *> &members = channel.getMembers();
    bool first = true;
    for (std::set<Client *>::iterator it = members.begin(); it != members.end(); it++) {
        if (!first) {
            message << " ";
        }
        first = false;
        if (channel.isOp(*it)) {
            message << "@" << (*it)->getNickname();
        } else {
            message << (*it)->getNickname();
        }
    }
    this->sendNumericReplyToClient(client, IRC::RPL_NAMREPLY, message.str(), params.str());
    this->sendNumericReplyToClient(client, IRC::RPL_ENDOFNAMES, IRC::MSG_ENDOFNAMES, channel.getName());
}
