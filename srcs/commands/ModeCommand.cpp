/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:20:45 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 18:16:34 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

#include <sstream>

const std::string ModeCommand::NAME = IRC::CMD_MODE;

ModeCommand::ModeCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &ModeCommand::getName(void) const {
    return ModeCommand::NAME;
}

bool ModeCommand::isValidModeFlag(const std::string &mode) {
    if (mode.size() != 2) {
        return false;
    }

    char sign = mode[0];
    char flag = mode[1];
    if (sign != '+' && sign != '-') {
        return false;
    }
    const std::string validFlags = "itkol";
    return validFlags.find(flag) != std::string::npos;
}

void ModeCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] ModeCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Check if user is registered
    if (!this->_sender->isRegistered()) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTREGISTERED, IRC::MSG_NOTREGISTERED);
        return;
    }

    // Check if at least two params
    if (this->_params.size() == 0) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    }

    // Get channel param
    Channel *channel = server.getChannelByName(this->_params[0]);
    if (channel == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, IRC::MSG_NOSUCHCHANNEL, this->_params[0]);
        return;
    }
    if (this->_params.size() == 1) {
        handleModeNoParam(server, channel, this->_sender);
        return;
    }

    // Get flag param
    const std::string &modeFlag = this->_params[1];
    if (!ModeCommand::isValidModeFlag(modeFlag)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_UNKNOWNMODE, IRC::MSG_UNKNOWNMODE + channel->getName(), modeFlag);
        return;
    }

    // Check if sender is authorized to execute MODE command
    if (!channel->isMember(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOTONCHANNEL, IRC::MSG_NOTONCHANNEL, channel->getName());
        return;
    }
    if (!channel->isOp(this->_sender)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_CHANOPRIVSNEEDED, IRC::MSG_CHANOPRIVSNEEDED, channel->getName());
        return;
    }

// Apply MODE command
#ifdef DEBUG
    std::cout << "[DEBUG MODE] " << this->_sender->getNickname() << " tries to set " << modeFlag << " on " << channel->getName() << std::endl;
#endif
    switch (modeFlag[1]) {
    case 'i':
        handleModeI(server, channel, modeFlag);
        break;
    case 't':
        handleModeT(server, channel, modeFlag);
        break;
    case 'k': {
        if (modeFlag == "+k" && this->_params.size() < 3) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
            return;
        }
        std::string key = this->_params.size() > 2 ? this->_params[2] : "";
        handleModeK(server, channel, modeFlag, key);
        break;
    }
    case 'o':
        // Check if third param is given
        if (this->_params.size() < 3) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
            return;
        }
        handleModeO(server, channel, modeFlag, this->_params[2]);
        break;
    case 'l': {
        // Check if third param is given
        if (modeFlag == "+l" && this->_params.size() < 3) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
            return;
        }
        std::string limitStr = this->_params.size() > 2 ? this->_params[2] : "";
        handleModeL(server, channel, modeFlag, limitStr);
        break;
    }
    default:
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_UNKNOWNMODE, IRC::MSG_UNKNOWNMODE + channel->getName(), modeFlag);
        break;
    }
}

void ModeCommand::handleModeI(Server &server, Channel *channel, const std::string &modeFlag) {
    bool enable = (modeFlag[0] == '+');
    channel->setInviteOnlyMode(enable);

    // Broadcast message to all channel members
    const std::string msg = this->getName() + " " + channel->getName() + " " + modeFlag;
    server.sendMessageToClient(this->_sender, this->_sender, msg, "");
    server.sendMessageToChannelUsers(*channel, this->_sender, msg, "");
}

void ModeCommand::handleModeT(Server &server, Channel *channel, const std::string &modeFlag) {
    bool enable = (modeFlag[0] == '+');
    channel->setTopicRestrictedMode(enable);

    // Broadcast message to all channel members
    const std::string msg = this->getName() + " " + channel->getName() + " " + modeFlag;
    server.sendMessageToClient(this->_sender, this->_sender, msg, "");
    server.sendMessageToChannelUsers(*channel, this->_sender, msg, "");
}

void ModeCommand::handleModeK(Server &server, Channel *channel, const std::string &modeFlag, const std::string &key) {
    bool enable = (modeFlag[0] == '+');

    if (enable) {
        if (!Channel::isValidKey(key)) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_BADCHANKEY, IRC::MSG_BADCHANKEY1, this->getName());
            return;
        }
        channel->setKey(key);
        channel->setKeyMode(true);
        std::string msg = this->getName() + " " + channel->getName() + " " + modeFlag + " " + key;
        server.sendMessageToClient(this->_sender, this->_sender, msg, "");
        server.sendMessageToChannelUsers(*channel, this->_sender, msg, "");
    } else {
        channel->setKey("");
        channel->setKeyMode(false);
        std::string msg = this->getName() + " " + channel->getName() + " " + modeFlag;
        server.sendMessageToClient(this->_sender, this->_sender, msg, "");
        server.sendMessageToChannelUsers(*channel, this->_sender, msg, "");
    }
}

void ModeCommand::handleModeO(Server &server, Channel *channel, const std::string &modeFlag, const std::string &nick) {
    bool enable = (modeFlag[0] == '+');

    // Check that target is a member of server and channel
    Client *target = server.getClientByNickname(nick);
    if (target == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHNICK, IRC::MSG_NOSUCHNICK, nick);
        return;
    }
    if (!channel->isMember(target)) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_USERNOTINCHANNEL, IRC::MSG_USERNOTINCHANNEL, nick + " " + channel->getName());
        return;
    }

    // Apply the mode
    if (enable) {
        channel->addOp(target);
    } else {
        channel->removeOp(target);
    }

    // Send confirm message
    std::string msg = this->getName() + " " + channel->getName() + " " + modeFlag + " " + nick;
    server.sendMessageToClient(this->_sender, this->_sender, msg, "");
    server.sendMessageToChannelUsers(*channel, this->_sender, msg, "");
}

void ModeCommand::handleModeL(Server &server, Channel *channel, const std::string &modeFlag, const std::string &limitStr) {
    bool enable = (modeFlag[0] == '+');

    if (enable) {
        // Get the limit as number
        int limit;
        try {
            limit = stringToInt(limitStr);
        } catch (const std::exception &e) {
            limit = -1;
        }
        if (limit <= 0) {
            server.sendNumericReplyToClient(this->_sender, IRC::ERR_UNKNOWNMODE, IRC::MSG_UNKNOWNMODE + channel->getName(), modeFlag);
            return;
        }
        channel->setLimitMode(true);
        channel->setMemberLimit(limit);

        // Send broadcast message
        std::string msg = this->getName() + " " + channel->getName() + " " + modeFlag + " " + limitStr;
        server.sendMessageToClient(this->_sender, this->_sender, msg, "");
        server.sendMessageToChannelUsers(*channel, this->_sender, msg, "");
    } else {
        channel->setLimitMode(false);
        channel->setMemberLimit(0);
        std::string msg = this->getName() + " " + channel->getName() + " " + modeFlag;
        server.sendMessageToClient(this->_sender, this->_sender, msg, "");
        server.sendMessageToChannelUsers(*channel, this->_sender, msg, "");
    }
}

void ModeCommand::handleModeNoParam(Server &server, Channel *channel, Client *sender) {
    std::string modes = "+";
    std::string params;

    if (channel->isInviteOnly())
        modes += "i";
    if (channel->isTopicRestricted())
        modes += "t";
    if (channel->hasKeyMode()) {
        modes += "k";
        if (channel->isOp(sender)) {
            params += " " + channel->getKey();
        }
    }
    if (channel->hasMemberLimit()) {
        modes += "l";
        std::stringstream ss;
        ss << channel->getMemberLimit();
        std::string limitStr = ss.str();
        params += " " + limitStr;
    }
    std::string msg = this->getName() + " " + channel->getName() + " " + modes + params;
    server.sendMessageToClient(this->_sender, this->_sender, msg, "");
}
