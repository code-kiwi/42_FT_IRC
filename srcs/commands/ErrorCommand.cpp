/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:28:58 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/02 00:34:51 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorCommand.hpp"
#include "IRCCommands.hpp"

#include <iostream>

const std::string ErrorCommand::NAME = IRC::CMD_ERROR;

ErrorCommand::ErrorCommand(Client *client, const std::string &initialName, const std::vector<std::string> &params, int code, const std::string &msg)
    : Command(client, params), _initialName(initialName), _code(code), _msg(msg) {}

void ErrorCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] ErrorCommand #" << this->_code << " executed for fd " << this->_sender->getFd() << std::endl;
#endif
    // Prepare the response to the client
    server.sendNumericReplyToClient(this->_sender, this->_code, this->_msg, this->_initialName);
}

const std::string &ErrorCommand::getName(void) const {
    return ErrorCommand::NAME;
}
