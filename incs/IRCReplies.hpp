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

const int RPL_ENDOFWHO = 315;
const std::string MSG_ENDOFWHO = "End of WHO list";

const int RPL_NOTOPIC = 331;
const std::string MSG_NOTOPIC = "No topic is set";

const int RPL_TOPIC = 332;

const int RPL_INVITING = 341;

const int RPL_WHOREPLY = 352;

const int RPL_NAMREPLY = 353;

const int RPL_ENDOFNAMES = 366;
const std::string MSG_ENDOFNAMES = "End of /NAMES list";

const int RPL_MOTD = 372;
const std::string MSG_MOTD = "- Welcome to our IRC server!";

const int RPL_MOTDSTART = 375;
const std::string MSG_MOTDSTART = "Message of the day -";

const int RPL_ENDOFMOTD = 376;
const std::string MSG_ENDOFMOTD = "End of /MOTD command.";

const int ERR_NOSUCHNICK = 401;
const std::string MSG_NOSUCHNICK = "No such nick/channel";

const int ERR_NOSUCHCHANNEL = 403;
const std::string MSG_NOSUCHCHANNEL = "No such channel";

const int ERR_CANNOTSENDTOCHAN = 404;
const std::string MSG_CANNOTSENDTOCHAN = "Cannot send to channel";

const int ERR_UNKNOWNCOMMAND = 421;
const std::string MSG_UNKNOWNCOMMAND = "Unknown command";

const int ERR_NONICKNAMEGIVEN = 431;
const std::string MSG_NONICKNAMEGIVEN = "No nickname given";

const int ERR_ERRONEUSNICKNAME = 432;
const std::string MSG_ERRONEUSNICKNAME = "Erroneous nickname";

const int ERR_NICKNAMEINUSE = 433;
const std::string MSG_NICKNAMEINUSE = "Nickname is already in use";

const int ERR_USERNOTINCHANNEL = 441;
const std::string MSG_USERNOTINCHANNEL = "They aren't on that channel";

const int ERR_NOTONCHANNEL = 442;
const std::string MSG_NOTONCHANNEL = "You're not on that channel";

const int ERR_USERONCHANNEL = 443;
const std::string MSG_NUSERONCHANNEL = "is already on channel";

const int ERR_NOTREGISTERED = 451;
const std::string MSG_NOTREGISTERED = "You have not registered";

const int ERR_NEEDMOREPARAMS = 461;
const std::string MSG_NEEDMOREPARAMS = "Not enough parameters";

const int ERR_ALREADYREGISTERED = 462;
const std::string MSG_ALREADYREGISTERED = "Unauthorized command (already registered)";

const int ERR_PASSWDMISMATCH = 464;
const std::string MSG_PASSWDMISMATCH = "Password incorrect";

const int ERR_CHANNELISFULL = 471;
const std::string MSG_CHANNELISFULL = "Cannot join channel (+l)";

const int ERR_UNKNOWNMODE = 472;
const std::string MSG_UNKNOWNMODE = "is unknown mode char to me for ";

const int ERR_INVITEONLYCHAN = 473;
const std::string MSG_INVITEONLYCHAN = "Cannot join channel (+i)";

const int ERR_BADCHANKEY = 475;
const std::string MSG_BADCHANKEY1 = "Bad Channel Key";
const std::string MSG_BADCHANKEY2 = "Cannot join channel (+k)";

const int ERR_BADCHANMASK = 476;
const std::string MSG_BADCHANMASK = "Bad Channel Mask";

const int ERR_CHANOPRIVSNEEDED = 482;
const std::string MSG_CHANOPRIVSNEEDED = "You're not channel operator";

} // namespace IRC

#endif
