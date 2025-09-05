/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:29:07 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/05 19:25:48 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORCOMMAND_HPP
#define ERRORCOMMAND_HPP

#include "Client.hpp"
#include "Command.hpp"

class ErrorCommand : public Command {
public:
    ErrorCommand(Client &client);
    void execute(Server &server);
};

#endif
