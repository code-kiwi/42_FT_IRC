/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCCommands.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 08:07:44 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/08 15:31:15 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCCOMMANDS_HPP
#define IRCCOMMANDS_HPP

#include <string>

namespace IRC {
const std::string CMD_PASS = "PASS";
const std::string CMD_NICK = "NICK";
const std::string CMD_USER = "USER";
const std::string CMD_CAP = "CAP";
const std::string CMD_ERROR = "ERROR";
const std::string CMD_JOIN = "JOIN";
const std::string CMD_PART = "PART";
const std::string CMD_QUIT = "QUIT";
const std::string CMD_MODE = "MODE";
const std::string CMD_INVITE = "INVITE";
const std::string CMD_KICK = "KICK";
const std::string CMD_PING = "PING";
const std::string CMD_PRIVMSG = "PRIVMSG";
const std::string CMD_TOPIC = "TOPIC";
const std::string CMD_WHO = "WHO";
const std::string CMD_NAMES = "NAMES";
const std::string CMD_LIST = "LIST";
} // namespace IRC

#endif
