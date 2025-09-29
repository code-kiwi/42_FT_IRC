/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCReplies.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:24:06 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/29 16:53:26 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCREPLIES_HPP
#define IRCREPLIES_HPP

#include <string>

namespace IRC {

const int RPL_WELCOME = 1;
const std::string MSG_WELCOME = "Welcome to the Internet Relay Network";

const int RPL_YOURHOST = 2;
const std::string MSG_YOURHOST1 = "Your host is";
const std::string MSG_YOURHOST2 = "running version";

const int RPL_CREATED = 3;
const std::string MSG_CREATED = "This server was created";

const int RPL_MYINFO = 4;

const int RPL_MOTD = 372;
const std::string MSG_MOTD = "- Welcome to our IRC server!";

const int RPL_MOTDSTART = 375;
const std::string MSG_MOTDSTART = "Message of the day -";

const int RPL_ENDOFMOTD = 376;
const std::string MSG_ENDOFMOTD = "End of /MOTD command.";

const int ERR_UNKNOWNCOMMAND = 421;
const std::string MSG_UNKNOWNCOMMAND = "Unknown command";

const int ERR_NONICKNAMEGIVEN = 431;
const std::string MSG_NONICKNAMEGIVEN = "No nickname given";

const int ERR_ERRONEUSNICKNAME = 432;
const std::string MSG_ERRONEUSNICKNAME = "Erroneous nickname";

const int ERR_NICKNAMEINUSE = 433;
const std::string MSG_NICKNAMEINUSE = "Nickname is already in use";

const int ERR_NEEDMOREPARAMS = 461;
const std::string MSG_NEEDMOREPARAMS = "Not enough parameters";

const int ERR_ALREADYREGISTERED = 462;
const std::string MSG_ALREADYREGISTERED = "Unauthorized command (already registered)";

const int ERR_PASSWDMISMATCH = 464;
const std::string MSG_PASSWDMISMATCH = "Password incorrect";

} // namespace IRC

#endif

/*



           ERR_UNAVAILRESOURCE             ERR_RESTRICTED
*/
