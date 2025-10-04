/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:20:45 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 11:39:05 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

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
    if (this->_params.size() < 2) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, IRC::MSG_NEEDMOREPARAMS, this->getName());
        return;
    }

    // Get and check params
    Channel *channel = server.getChannelByName(this->_params[0]);
    const std::string &modeFlag = this->_params[1];
    if (channel == NULL) {
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, IRC::MSG_NOSUCHCHANNEL, this->_params[0]);
        return;
    }
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
        std::cout << "MODE i" << std::endl;
        break;
    case 't':
        std::cout << "MODE t" << std::endl;
        break;
    case 'k':
        std::cout << "MODE k" << std::endl;
        break;
    case 'o':
        std::cout << "MODE o" << std::endl;
        break;
    case 'l':
        std::cout << "MODE l" << std::endl;
        break;
    default:
        server.sendNumericReplyToClient(this->_sender, IRC::ERR_UNKNOWNMODE, IRC::MSG_UNKNOWNMODE + channel->getName(), modeFlag);
        break;
    }
}
