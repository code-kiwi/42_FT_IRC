/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:30:36 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/17 17:51:46 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASSCOMMAND_HPP
#define PASSCOMMAND_HPP

#include "Client.hpp"
#include "Command.hpp"

#include <vector>

class PassCommand : public Command {
public:
    static const std::string NAME;
    PassCommand(Client &sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    PassCommand(Client &client);
};

#endif
