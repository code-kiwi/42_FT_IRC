/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:24:36 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:26:47 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSGCOMMAND_HPP
#define PRIVMSGCOMMAND_HPP

#include "Command.hpp"

class PrivMsgCommand : public Command {
public:
    static const std::string NAME;
    PrivMsgCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    PrivMsgCommand(Client *client);
};

#endif
