/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 07:05:27 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 07:06:47 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAPCOMMAND_HPP
#define CAPCOMMAND_HPP

#include "Command.hpp"

class CapCommand : public Command {
public:
    static const std::string NAME;
    CapCommand(Client &sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    CapCommand(Client &client);
};

#endif
