/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbutel <lbutel@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:43:05 by lbutel            #+#    #+#             */
/*   Updated: 2025/09/26 21:44:40 by lbutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

#include <sstream>

const std::string JoinCommand::NAME = IRC::CMD_JOIN;

JoinCommand::JoinCommand(Client &sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &JoinCommand::getName(void) const {
    return JoinCommand::NAME;
}

void JoinCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] JoinCommand executed for fd " << this->_sender.getFd() << std::endl;
#endif
	if (this->_params.empty())
	{
		std::ostringstream oss;
		oss << this->getName();
		oss << " :" << IRC::MSG_NEEDMOREPARAMS;
		server.sendNumericReplyToClient(this->_sender, IRC::ERR_NEEDMOREPARAMS, oss.str());
		return ;
	}
	std::vector<std::string> channel_names = splitString(this->_params[0], ",");
	std::vector<std::string> channel_keys;
	if (this->_params.size() >= 2)
		channel_keys = splitString(this->_params[1], ",");
	for (size_t i = 0 ; i < channel_names.size() ; i++)
	{
		std::string channel_name = channel_names[i];
		if (!Channel::isValidName(channel_name))
		{
			std::ostringstream oss;
			oss << this->getName() << " ";
			oss << channel_name;
			oss << " :" << IRC::MSG_BADCHANMASK;
			server.sendNumericReplyToClient(this->_sender, IRC::ERR_BADCHANMASK, oss.str());
		}
		else if (server.isChannelCreated(channel_name))
		{
			Channel *channel = server.getChannelByName(channel_name);
			if (channel->isMember(this->_sender))
			{
				std::cout << "already a member of " << channel->getName() << std::endl;
			}
			else
			{
				channel->addMember(this->_sender);
				server.sendMessageToChannel(*channel, this->_sender, *this, channel_name);
				std::cout << "welcome to " << channel->getName() << std::endl;
				server.sendNamesToClient(this->_sender, *channel);
			}
		}
		else
		{
			Channel &new_channel = server.addChannel(channel_name, this->_sender);
			new_channel.addMember(this->_sender);
			server.sendMessageToChannel(new_channel, this->_sender, *this, channel_name);
			std::cout << "created " << new_channel.getName() << std::endl;
			server.sendNamesToClient(this->_sender, new_channel);
		}
	}
}
