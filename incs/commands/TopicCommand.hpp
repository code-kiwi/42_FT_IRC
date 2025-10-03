/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:24:41 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/04 01:26:44 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPICCOMMAND_HPP
#define TOPICCOMMAND_HPP

#include "Command.hpp"

class TopicCommand : public Command {
public:
    static const std::string NAME;
    TopicCommand(Client *sender, const std::vector<std::string> &params);

    const std::string &getName(void) const;

    void execute(Server &server);

private:
    TopicCommand(Client *client);
};

#endif
