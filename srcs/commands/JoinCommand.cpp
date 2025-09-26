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
	(void) server;
}
