/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 07:07:08 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 07:11:46 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CapCommand.hpp"
#include "IRCCommands.hpp"

const std::string CapCommand::NAME = IRC::CMD_CAP;

CapCommand::CapCommand(Client &sender, const std::vector<std::string> &params)
    : Command(sender, params) {}

const std::string &CapCommand::getName(void) const {
    return CapCommand::NAME;
}

void CapCommand::execute(Server &server) {
#ifdef DEBUG
    std::cout << "[DEBUG] CapCommand executed for fd " << this->_sender.getFd() << std::endl;
#endif
    if (!this->_params.empty() && this->_params[0] == "LS") {
        // Send an empty list of capabilities
        this->_sender.appendToOutputBuffer(":" + server.getName() + " CAP * LS :\r\n");
        server.markClientForWrite(this->_sender.getFd());
    }
}
