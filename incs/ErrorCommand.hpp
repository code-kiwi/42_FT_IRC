/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:29:07 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/06 02:00:52 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORCOMMAND_HPP
#define ERRORCOMMAND_HPP

#include "Client.hpp"
#include "Command.hpp"

#include <vector>

class ErrorCommand : public Command {
public:
    ErrorCommand(Client &client);
    ErrorCommand(Client &client, std::vector<std::string> params);
    void execute(Server &server);
};

#endif
