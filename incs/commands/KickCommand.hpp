/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:24:15 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/06 12:31:10 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICKCOMMAND_HPP
#define KICKCOMMAND_HPP

#include "Command.hpp"

class KickCommand : public Command {
public:
    static const std::string NAME;
    KickCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    KickCommand(Client *client);
    void kickMemberFromChannel(Server &server, Channel *channel, const std::string &targetName, const std::string &comment);
    Channel *getAndCheckChannel(Server &server, const std::string &channelName);
};

#endif
