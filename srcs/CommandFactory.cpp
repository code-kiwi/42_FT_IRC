/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:23:15 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/05 18:15:23 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandFactory.hpp"
#include "ErrorCommand.hpp"

Command *CommandFactory::createCommand(const std::string &rawCommand, Client &client) {
    ParsedCommand parsedCommand = CommandFactory::parseRawCommand(rawCommand);

    if (!parsedCommand.isValid) {
        return new ErrorCommand(client);
    }
    return new ErrorCommand(client);
}

CommandFactory::ParsedCommand::ParsedCommand(void) : isValid(false) {}

CommandFactory::ParsedCommand CommandFactory::parseRawCommand(const std::string &rawCommand) {
    ParsedCommand parsedCommand;
    (void)rawCommand;
    return parsedCommand;
}