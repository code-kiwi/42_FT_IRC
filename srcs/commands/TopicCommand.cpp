/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:28:22 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:30:17 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string TopicCommand::NAME = IRC::CMD_TOPIC;

TopicCommand::TopicCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &TopicCommand::getName(void) const {
    return TopicCommand::NAME;
}

void TopicCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] TopicCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    (void)server;
}