/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbutel <lbutel@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:41:27 by lbutel            #+#    #+#             */
/*   Updated: 2025/09/26 21:42:43 by lbutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOINCOMMAND_HPP
#define JOINCOMMAND_HPP

#include "Client.hpp"
#include "Command.hpp"

#include <vector>

class JoinCommand : public Command {
public:
    static const std::string NAME;
    JoinCommand(Client &sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    JoinCommand(Client &client);
};

#endif /* JOINCOMMAND_HPP */
