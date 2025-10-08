/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NamesCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:30:29 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/06 13:30:55 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NAMESCOMMAND_HPP
#define NAMESCOMMAND_HPP

#include "Command.hpp"

class NamesCommand : public Command {
public:
    static const std::string NAME;
    NamesCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    NamesCommand(Client *client);
};

#endif
