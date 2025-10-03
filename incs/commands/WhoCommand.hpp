/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:24:46 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:30:54 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHOCOMMAND_HPP
#define WHOCOMMAND_HPP

#include "Command.hpp"

class WhoCommand : public Command {
public:
    static const std::string NAME;
    WhoCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    WhoCommand(Client *client);
};

#endif
