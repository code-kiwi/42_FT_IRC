/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:28:58 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/16 18:33:24 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorCommand.hpp"

#include <iostream>
#include <sstream>

const std::string ErrorCommand::NAME = "ERROR";

ErrorCommand::ErrorCommand(Client &client, const std::vector<std::string> &params, int code, const std::string &msg)
    : Command(client, params), _code(code), _msg(msg) {}

void ErrorCommand::execute(Server &server) {
    std::ostringstream oss;

#ifdef DEBUG
    std::cout << "[DEBUG] ErrorCommand #" << this->_code << " executed for fd " << this->_sender.getFd() << std::endl;
#endif

    // Build the response string
    oss << ":" << server.getName()
        << " " << this->_code
        << " ";
    if (this->_sender.getNickname().empty()) {
        oss << "*";
    } else {
        oss << this->_sender.getNickname();
    }
    for (size_t i = 0; i < this->_params.size(); i++) {
        oss << " " << this->_params[i];
    }
    oss << " :" << this->_msg << "\r\n";

    // Save the response and indicate that is has to be sent
    this->_sender.appendToOutputBuffer(oss.str());
    server.markClientForWrite(this->_sender.getFd());
}

const std::string &ErrorCommand::getName() const {
    return ErrorCommand::NAME;
}
