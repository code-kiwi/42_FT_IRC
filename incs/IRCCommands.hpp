/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCCommands.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 08:07:44 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/03 12:22:50 by mhotting         ###   ########.fr       */
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
} // namespace IRC

#endif
