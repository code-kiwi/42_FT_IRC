/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:23:36 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:26:54 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITECOMMAND_HPP
#define INVITECOMMAND_HPP

#include "Command.hpp"

class InviteCommand : public Command {
public:
    static const std::string NAME;
    InviteCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    InviteCommand(Client *client);
};

#endif
