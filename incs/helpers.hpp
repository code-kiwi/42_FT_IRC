/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:33:47 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/18 20:28:52 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>

int stringToInt(const std::string &str);
void validatePort(int port);
void validatePassword(const std::string &password);
std::string formatNumericReply(const std::string &serverName, int code, const std::string &target, const std::string &message);
std::string formatNumericReply(const std::string &serverName, int code, const std::string &target, const std::string &param, const std::string &message);
bool isLetter(char c);
bool isDigit(char c);
bool isSpecial(char c);
bool isValidNickname(const std::string &nick);

#endif
