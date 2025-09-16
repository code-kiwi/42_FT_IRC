/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:23:15 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/16 18:33:05 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandFactory.hpp"
#include "ErrorCommand.hpp"
#include "IRCReplies.hpp"

#include <vector>

Command *CommandFactory::createCommand(const std::string &rawCommand, Client &client) {
    ParsedCommand parsedCommand = CommandFactory::parseRawCommand(rawCommand);

    if (!parsedCommand.isValid) {
        return new ErrorCommand(client, parsedCommand.params, IRC::ERR_UNKNOWNCOMMAND, IRC::MSG_UNKNOWNCOMMAND);
    }
    return new ErrorCommand(client, parsedCommand.params, IRC::ERR_UNKNOWNCOMMAND, IRC::MSG_UNKNOWNCOMMAND);
}

CommandFactory::ParsedCommand::ParsedCommand(void)
    : isValid(false) {}

CommandFactory::ParsedCommand CommandFactory::parseRawCommand(const std::string &rawCommand) {
    ParsedCommand result;
    result.isValid = false;
    std::string line = rawCommand;

    // First validity check
    if (line.empty()) {
        return result;
    }

    // Extract the prefix
    if (line[0] == ':') {
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            result.prefix = line.substr(1, spacePos - 1);
            line.erase(0, spacePos + 1);
        } else {
            return result;
        }
    }

    // Second validity check
    if (line.empty()) {
        return result;
    }

    // Extract the command name
    size_t spacePos = line.find(' ');
    if (spacePos != std::string::npos) {
        result.commandName = line.substr(0, spacePos);
        line.erase(0, spacePos + 1);
    } else {
        result.commandName = line;
        line.clear();
    }

    // Extract parameters
    while (!line.empty()) {
        if (line[0] == ':') {
            // Get the trailing parameter which can contain spaces
            result.params.push_back(line.substr(1));
            break;
        }
        spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            result.params.push_back(line.substr(0, spacePos));
            line.erase(0, spacePos + 1);
        } else {
            result.params.push_back(line);
            break;
        }
    }

    result.isValid = result.commandName.empty();
    return result;
}
