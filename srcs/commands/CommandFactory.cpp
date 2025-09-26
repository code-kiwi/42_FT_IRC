/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:23:15 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 07:13:26 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandFactory.hpp"
#include "CapCommand.hpp"
#include "ErrorCommand.hpp"
#include "IRCCommands.hpp"
#include "IRCReplies.hpp"
#include "NickCommand.hpp"
#include "PassCommand.hpp"
#include "UserCommand.hpp"
#include "JoinCommand.hpp"

#include <vector>

Command *CommandFactory::createCommand(const std::string &rawCommand, Client &client) {
    ParsedCommand parsed = CommandFactory::parseRawCommand(rawCommand);

#ifdef DEBUG
    std::cout << "[DEBUG] Parsed Command " << parsed.commandName << ", params : [";
    for (size_t i = 0; i < parsed.params.size(); i++) {
        std::cout << parsed.params[i];
        if (i < parsed.params.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "], isValid: " << std::boolalpha << parsed.isValid << std::endl;
#endif

    // Check command validity
    if (!parsed.isValid) {
        return new ErrorCommand(client, parsed.commandName, parsed.params, IRC::ERR_UNKNOWNCOMMAND, IRC::MSG_UNKNOWNCOMMAND);
    }

    // Search for the correct command
    static CommandFactory::CreatorMap registry = CommandFactory::initRegistry();
    std::map<std::string, CommandCreator>::iterator it = registry.find(parsed.commandName);
    if (it != registry.end())
        return it->second(client, parsed.params);

    // Unknown command detected
    return new ErrorCommand(client, parsed.commandName, parsed.params, IRC::ERR_UNKNOWNCOMMAND, IRC::MSG_UNKNOWNCOMMAND);
}

// Parsed command constructor
CommandFactory::ParsedCommand::ParsedCommand(void)
    : isValid(false) {}

// Command parser
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

    result.isValid = !result.commandName.empty();
    return result;
}

// Initializes the registry
std::map<std::string, CommandFactory::CommandCreator> CommandFactory::initRegistry(void) {
    std::map<std::string, CommandCreator> m;

    m[IRC::CMD_PASS] = &CommandFactory::createPassCommand;
    m[IRC::CMD_NICK] = &CommandFactory::createNickCommand;
    m[IRC::CMD_USER] = &CommandFactory::createUserCommand;
    m[IRC::CMD_CAP] = &CommandFactory::createCapCommand;
    m[IRC::CMD_JOIN] = &CommandFactory::createJoinCommand;

    return m;
}

Command *CommandFactory::createPassCommand(Client &client, const std::vector<std::string> &params) {
    return new PassCommand(client, params);
}

Command *CommandFactory::createNickCommand(Client &client, const std::vector<std::string> &params) {
    return new NickCommand(client, params);
}

Command *CommandFactory::createUserCommand(Client &client, const std::vector<std::string> &params) {
    return new UserCommand(client, params);
}

Command *CommandFactory::createCapCommand(Client &client, const std::vector<std::string> &params) {
    return new CapCommand(client, params);
}

Command *CommandFactory::createJoinCommand(Client &client, const std::vector<std::string> &params) {
    return new JoinCommand(client, params);
}
