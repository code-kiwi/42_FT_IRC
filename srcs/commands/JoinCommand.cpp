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
		if (!server.isChannelCreated(channel_names[i]))
		{
			std::ostringstream oss;
			oss << this->getName() << " ";
			oss << channel_names[i];
			oss << " :" << IRC::MSG_NOSUCHCHANNEL;
			server.sendNumericReplyToClient(this->_sender, IRC::ERR_NOSUCHCHANNEL, oss.str());
			// return ;
		}
	}
}
