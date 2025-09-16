/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCCommands.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 08:07:44 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/19 07:08:04 by mhotting         ###   ########.fr       */
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
} // namespace IRC

#endif
