/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:26:30 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/06 02:16:38 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDFACTORY_HPP
#define COMMANDFACTORY_HPP

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class ErrorCommand;

class CommandFactory {
public:
    static Command *createCommand(const std::string &rawCommand, Client &client);

private:
    CommandFactory(void);

    struct ParsedCommand {
        std::string prefix;
        std::string commandName;
        std::vector<std::string> params;
        bool isValid;

        ParsedCommand(void);
    };

    static ParsedCommand parseRawCommand(const std::string &rawCommand);
};

#endif
