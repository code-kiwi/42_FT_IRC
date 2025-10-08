/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 03:46:42 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/03 04:01:09 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QuitCommand.hpp"

#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "helpers.hpp"

const std::string QuitCommand::NAME = IRC::CMD_QUIT;

QuitCommand::QuitCommand(Client *sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &QuitCommand::getName(void) const {
    return QuitCommand::NAME;
}

void QuitCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] QuitCommand executed for fd " << this->_sender->getFd() << std::endl;
#endif
    std::string reason = "";
    if (this->_params.size() > 0) {
        reason = this->_params[0];
    }

    server.clearClient(this->_sender->getFd(), reason);
}
