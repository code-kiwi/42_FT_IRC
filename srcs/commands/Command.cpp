/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:34:43 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 05:35:39 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command(Client &client)
    : _sender(client), _params(){};

Command::Command(Client &client, const std::vector<std::string> &params)
    : _sender(client), _params(params) {}

Command::~Command(void) {}

const std::vector<std::string> &Command::getParams(void) const {
    return this->_params;
}

const Client &Command::getSender(void) const {
    return this->_sender;
}

std::ostream &operator<<(std::ostream &o, const Command &command) {
    const std::vector<std::string> &params = command.getParams();
    const Client &sender = command.getSender();

    o << "COMMAND : '" << command.getName() << "' - Sender <" << sender.getFd() << "> - Params : ";
    if (params.empty()) {
        o << "no params";
    } else {
        o << "[";
        for (size_t i = 0; i < params.size(); i++) {
            o << i << " : '" << params[i] << "'";
            if (i < params.size() - 1) {
                o << ", ";
            } else {
                o << "]";
            }
        }
    }
    return o;
}
