/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:29:07 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/18 06:03:15 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORCOMMAND_HPP
#define ERRORCOMMAND_HPP

#include "Client.hpp"
#include "Command.hpp"

#include <vector>

class ErrorCommand : public Command {
public:
    static const std::string NAME;
    ErrorCommand(Client &client, const std::string &initialName, const std::vector<std::string> &params, int code, const std::string &msg);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    std::string _initialName;
    int _code;
    std::string _msg;

    ErrorCommand(Client &client);
    ErrorCommand(Client &client, std::vector<std::string> params);
};

#endif
