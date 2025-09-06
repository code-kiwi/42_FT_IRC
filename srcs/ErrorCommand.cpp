/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:28:58 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/06 02:01:28 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorCommand.hpp"

#include <iostream>

ErrorCommand::ErrorCommand(Client &client) : Command(client) {}

ErrorCommand::ErrorCommand(Client &client, std::vector<std::string> params) : Command(client, params) {}

void ErrorCommand::execute(Server &server) {
    (void)server;
    std::cout << "Execution of Error Command" << std::endl;
}
