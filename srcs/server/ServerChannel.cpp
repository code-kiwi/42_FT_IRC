/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:06:06 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/10 00:54:10 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCReplies.hpp"
#include "Server.hpp"

std::vector<std::string> Server::getAllChannelNames(void) const {
    std::vector<std::string> channelNames;

    for (size_t i = 0; i < this->_channels.size(); i++) {
        channelNames.push_back(this->_channels[i].getName());
    }
    return channelNames;
}

const std::vector<Channel> &Server::getChannels(void) const {
    return this->_channels;
}

Channel &Server::addChannel(const std::string &channelName) {
    Channel channel(channelName);
    this->_channels.push_back(channel);

    return (this->_channels.back());
}

Channel *Server::getChannelByName(const std::string channelName) {
    for (size_t i = 0; i < this->_channels.size(); i++) {
        if (this->_channels[i].getName() == channelName)
            return (&this->_channels[i]);
    }
    return (NULL);
}

bool Server::isChannelCreated(const std::string channelName) {
    for (size_t i = 0; i < this->_channels.size(); i++) {
        if (this->_channels[i].getName() == channelName)
            return (true);
    }
    return (false);
}

void Server::removeChannel(const std::string &channelName) {
    for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++) {
        if (it->getName() == channelName) {
            this->_channels.erase(it);
            break;
        }
    }
}

void Server::partClientFromAllChannels(Client *client, const std::string &reason, const std::string &commandName) {
    for (size_t i = 0; i < this->_channels.size();) {
        size_t oldSize = this->_channels.size();
        partClientFromChannel(client, this->_channels[i], reason, commandName);
        if (_channels.size() < oldSize) {
            continue; // A channel has been removed, next channel is now at index i
        }
        ++i;
    }
}

void Server::partClientFromChannel(Client *client, Channel &channel, const std::string &reason, const std::string &commandName) {
    if (!channel.isMember(client)) {
        if (commandName == IRC::CMD_PART) {
            this->sendNumericReplyToClient(client, IRC::ERR_NOTONCHANNEL, IRC::MSG_NOTONCHANNEL, channel.getName());
        }
        return;
    }

    // Broadcast message
    this->sendMessageToClient(client, client, commandName + " " + channel.getName(), reason);
    this->sendMessageToChannelUsers(channel, client, commandName + " " + channel.getName(), reason);

    // Remove user
    channel.removeMember(client);

    // Handle operator
    if (channel.isOp(client)) {
        channel.removeOp(client);
        if (channel.getOpsCount() == 0 && channel.getMemberCount() > 0) {
            channel.promoteMemberIfNecessary(*this, client);
        }
    }

    // Remove channel if empty
    if (channel.getMemberCount() == 0) {
        this->removeChannel(channel.getName());
    }
}

void Server::quitClientFromAllChannels(Client *client, const std::string &reason) {
    std::set<Client *> targets;

    size_t i = 0;
    while (i < this->_channels.size()) {
        Channel &channel = this->_channels[i];
        if (!channel.isMember(client)) {
            i++;
            continue;
        }
        targets.insert(channel.getMembers().begin(), channel.getMembers().end());

        // Remove client from the channel
        channel.removeMember(client);
        if (channel.isOp(client)) {
            channel.removeOp(client);
            if (channel.getOpsCount() == 0 && channel.getMemberCount() > 0) {
                channel.promoteMemberIfNecessary(*this, client);
            }
        }
        channel.removeInvited(client);

        // Remove channel if empty
        if (channel.getMemberCount() == 0) {
            this->removeChannel(channel.getName());
            continue;
        }
        i++;
    }
    targets.erase(client);

    // Send broadcast message
    for (std::set<Client *>::iterator it = targets.begin(); it != targets.end(); it++) {
        this->sendMessageToClient(client, *it, IRC::CMD_QUIT, reason);
    }
}

std::set<Client *> Server::getChannelPeers(Client *client) {
    std::set<Client *> peers;

    for (size_t i = 0; i < this->_channels.size(); i++) {
        Channel &channel = this->_channels[i];
        if (!channel.isMember(client)) {
            continue;
        }
        std::set<Client *> members = channel.getMembers();
        peers.insert(members.begin(), members.end());
    }
    peers.erase(client);
    return peers;
}
