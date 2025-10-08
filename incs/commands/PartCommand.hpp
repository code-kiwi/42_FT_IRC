/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 03:14:33 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/03 03:19:04 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTCOMMAND_HPP
#define PARTCOMMAND_HPP

#include "Command.hpp"

class PartCommand : public Command {
public:
    static const std::string NAME;
    PartCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    PartCommand(Client *client);
};

#endif
