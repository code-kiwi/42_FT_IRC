/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:26:30 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 15:32:06 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDFACTORY_HPP
#define COMMANDFACTORY_HPP

#include "Client.hpp"
#include "Command.hpp"

#include <map>
#include <string>

class ErrorCommand;

class CommandFactory {
public:
    // Interface
    static Command *createCommand(const std::string &rawCommand, Client *client);

private:
    CommandFactory(void);

    // Store the information from a string command
    struct ParsedCommand {
        std::string prefix;
        std::string commandName;
        std::vector<std::string> params;
        bool isValid;
        ParsedCommand(void);
    };

    static ParsedCommand parseRawCommand(const std::string &rawCommand);

    // Registry utils
    typedef Command *(*CommandCreator)(Client *client, const std::vector<std::string> &params);
    typedef std::map<std::string, CommandCreator> CreatorMap;
    static std::map<std::string, CommandCreator> initRegistry(void);

    // Create commands
    static Command *createPassCommand(Client *client, const std::vector<std::string> &params);
    static Command *createNickCommand(Client *client, const std::vector<std::string> &params);
    static Command *createUserCommand(Client *client, const std::vector<std::string> &params);
    static Command *createCapCommand(Client *client, const std::vector<std::string> &params);
    static Command *createJoinCommand(Client *client, const std::vector<std::string> &params);
    static Command *createPartCommand(Client *client, const std::vector<std::string> &params);
    static Command *createQuitCommand(Client *client, const std::vector<std::string> &params);
    static Command *createModeCommand(Client *client, const std::vector<std::string> &params);
    static Command *createInviteCommand(Client *client, const std::vector<std::string> &params);
    static Command *createKickCommand(Client *client, const std::vector<std::string> &params);
    static Command *createPingCommand(Client *client, const std::vector<std::string> &params);
    static Command *createPrivMsgCommand(Client *client, const std::vector<std::string> &params);
    static Command *createTopicCommand(Client *client, const std::vector<std::string> &params);
    static Command *createWhoCommand(Client *client, const std::vector<std::string> &params);
    static Command *createNamesCommand(Client *client, const std::vector<std::string> &params);
    static Command *createListCommand(Client *client, const std::vector<std::string> &params);
};

#endif
