/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:20:49 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/06 00:29:55 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODECOMMAND_HPP
#define MODECOMMAND_HPP

#include "Command.hpp"

class ModeCommand : public Command {
public:
    static const std::string NAME;
    ModeCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    static bool isValidModeFlag(const std::string &mode);
    ModeCommand(Client *client);
    void handleModeI(Server &server, Channel *channel, const std::string &modeFlag);
    void handleModeT(Server &server, Channel *channel, const std::string &modeFlag);
    void handleModeK(Server &server, Channel *channel, const std::string &modeFlag, const std::string &key);
    void handleModeO(Server &server, Channel *channel, const std::string &modeFlag, const std::string &nick);
    void handleModeL(Server &server, Channel *channel, const std::string &modeFlag, const std::string &limitStr);
    void handleModeNoParam(Server &server, Channel *channel, Client *sender);
};

#endif
