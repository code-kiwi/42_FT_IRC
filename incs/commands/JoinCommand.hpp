/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:41:27 by lbutel            #+#    #+#             */
/*   Updated: 2025/10/05 15:10:28 by mhotting         ###   ########.fr       */
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
    JoinCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    JoinCommand(Client *client);
    void sendJoinReplies(Server &server, Channel &channel);
    void joinExistingChannel(Server &server, const std::string &channelName, const std::string &channelKey);
    void joinNewChannel(Server &server, const std::string &channelName, const std::string &channelKey);
};

#endif
