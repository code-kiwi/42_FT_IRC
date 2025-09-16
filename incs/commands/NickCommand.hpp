/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:49:01 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/18 18:52:55 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICKCOMMAND_HPP
#define NICKCOMMAND_HPP

#include "Client.hpp"
#include "Command.hpp"

#include <vector>

class NickCommand : public Command {
public:
    static const std::string NAME;
    NickCommand(Client &sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    NickCommand(Client &client);
};

#endif
