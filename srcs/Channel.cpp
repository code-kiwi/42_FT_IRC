/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:14:31 by lbutel            #+#    #+#             */
/*   Updated: 2025/10/10 00:54:29 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"

#include <stdexcept>

Channel::Channel(const std::string &name)
    : _name(name), _key(""), _topic(""), _inviteOnly(false), _topicRestricted(false), _keyMode(false), _limitMode(false), _limit(0) {}

Channel::~Channel(void) {}

bool Channel::isValidName(const std::string &channel_name) {
    if (channel_name.empty())
        return false;
    if (channel_name.length() < 2 || channel_name.length() > 50)
        return false;
    if (channel_name[0] != '#' && channel_name[0] != '&')
        return false;
    if (channel_name.find(' ') != std::string::npos)
        return false;
    if (channel_name.find('\a') != std::string::npos)
        return false;
    if (channel_name.find(',') != std::string::npos)
        return false;
    return true;
}

bool Channel::isValidKey(const std::string &channelKey) {
    if (channelKey.length() > 50)
        return false;
    if (channelKey.find(' ') != std::string::npos)
        return false;
    if (channelKey.find('\a') != std::string::npos)
        return false;
    if (channelKey.find(',') != std::string::npos)
        return false;
    return true;
}

const std::string &Channel::getName(void) const {
    return this->_name;
}

size_t Channel::getMemberCount(void) const {
    return this->_members.size();
}

std::set<Client *> &Channel::getMembers(void) {
    return this->_members;
}

const std::string &Channel::getKey(void) const {
    return this->_key;
}

void Channel::setKey(const std::string &key) {
    this->_key = key;
}

const std::string &Channel::getTopic(void) const {
    return this->_topic;
}

void Channel::setTopic(const std::string &topic) {
    this->_topic = topic;
}

bool Channel::isInviteOnly(void) const {
    return this->_inviteOnly;
}

bool Channel::isTopicRestricted(void) const {
    return this->_topicRestricted;
}

bool Channel::hasKeyMode(void) const {
    return this->_keyMode;
}

bool Channel::hasMemberLimit(void) const {
    return this->_limitMode;
}

size_t Channel::getMemberLimit(void) const {
    return this->_limit;
}

void Channel::setInviteOnlyMode(bool val) {
    this->_inviteOnly = val;
}

void Channel::setTopicRestrictedMode(bool val) {
    this->_topicRestricted = val;
}

void Channel::setKeyMode(bool val) {
    this->_keyMode = val;
}

void Channel::setLimitMode(bool val) {
    this->_limitMode = val;
}

void Channel::setMemberLimit(size_t limit) {
    this->_limit = limit;
}

bool Channel::isMember(Client *client) const {
    return (this->_members.find(client) != this->_members.end());
}

void Channel::addMember(Client *client) {
    this->_members.insert(client);
}

void Channel::removeMember(Client *client) {
    this->_members.erase(client);
}

Client *Channel::getFirstMember(void) const {
    if (this->_members.empty()) {
        return NULL;
    }
    return *(this->_members.begin());
}

bool Channel::isOp(Client *client) const {
    return (this->_ops.find(client) != this->_ops.end());
}

void Channel::addOp(Client *client) {
    this->_ops.insert(client);
}

void Channel::removeOp(Client *client) {
    this->_ops.erase(client);
}

size_t Channel::getOpsCount(void) const {
    return this->_ops.size();
}

bool Channel::isInvited(Client *client) const {
    return (this->_invitedClients.find(client) != this->_invitedClients.end());
}

void Channel::addInvited(Client *client) {
    this->_invitedClients.insert(client);
}

void Channel::removeInvited(Client *client) {
    this->_invitedClients.erase(client);
}

void Channel::kickMember(Server &server, Client *client, Client *sourceClient) {
    if (client == NULL) {
        return;
    }
    this->removeMember(client);
    this->removeOp(client);
    this->removeInvited(client);
    this->promoteMemberIfNecessary(server, sourceClient);
}

void Channel::promoteMemberIfNecessary(Server &server, Client *sourceClient) {
    if (this->getOpsCount() > 0 || this->getMemberCount() == 0) {
        return;
    }
    Client *newOp = this->getFirstMember();
    if (newOp == NULL) {
        return;
    }
    this->addOp(newOp);
    std::string msg = IRC::CMD_MODE + " " + this->getName() + " +o " + newOp->getNickname();
    server.sendMessageToChannelUsers(*this, sourceClient, msg, "");
}
