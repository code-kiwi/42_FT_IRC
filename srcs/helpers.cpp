/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:34:38 by mhotting          #+#    #+#             */
/*   Updated: 2025/10/03 01:56:25 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.hpp"
#include "config.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

/**
 * @brief Convert a given string to an integer
 * @param str A const reference to the string to convert
 * @throws runtime_error
 */
int stringToInt(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        throw std::runtime_error("Invalid port - Conversion to int is not possible");
    }
    return value;
}

/**
 * @brief Throws an exception if the given port is not valid
 * @param port The port to validate
 * @throws invalid_argument
 */
void validatePort(int port) {
    if (port < MIN_PORT || port > MAX_PORT) {
        std::ostringstream oss;
        oss << "Invalid port - its value must be between " << MIN_PORT << " and " << MAX_PORT;
        throw std::invalid_argument(oss.str());
    }
}

/**
 * @brief Throws an exception if the given password is not valid
 * @param password The password to validate
 * @throws invalid_argument
 */
void validatePassword(const std::string &password) {
    if (password.empty()) {
        throw std::invalid_argument("Invalid password - Password cannot be empty");
    }
}

/**
 * @brief Prepares an IRC numeric reply as an std::string
 * @param serverName the name of the server
 * @param code the IRC official reply code
 * @param target the name of the message's target
 * @param message the message to deliver
 * @return the reply as an std::string
 */
std::string formatNumericReply(const std::string &serverName, int code, const std::string &target, const std::string &message) {
    std::ostringstream oss;
    oss << ":" << serverName << " "
        << std::setw(3) << std::setfill('0') << code << " "
        << target << " :" << message << "\r\n";
    return oss.str();
}

/**
 * @brief Prepares an IRC numeric reply as an std::string
 * @param serverName the name of the server
 * @param code the IRC official reply code
 * @param target the name of the message's target
 * @param param an additionnal parameter to append to the reply
 * @param message the message to deliver
 * @return the reply as an std::string
 */
std::string formatNumericReply(const std::string &serverName, int code, const std::string &target, const std::string &param, const std::string &message) {
    std::ostringstream oss;
    if (message.empty()) {
        oss << ":" << serverName << " "
            << std::setw(3) << std::setfill('0') << code << " " << target << " " << param << "\r\n";
    } else {
        oss << ":" << serverName << " "
            << std::setw(3) << std::setfill('0') << code << " "
            << target << " " << param << " :" << message << "\r\n";
    }
    return oss.str();
}

std::string formatMessage(const Client *client, const std::string &command_name, const std::string &params) {
    std::ostringstream oss;
    oss << ":" << client->getNickname() << "!" << client->getUsername() << "@" << client->getIpAddress() << " " << command_name;
    if (!params.empty()) {
        oss << " :" << params;
    }
    oss << "\r\n";
    return oss.str();
}

bool isLetter(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

bool isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

bool isSpecial(char c) {
    return c == '[' || c == ']' || c == '\\' || c == '`' ||
           c == '^' || c == '{' || c == '}';
}

bool isValidNickname(const std::string &nick) {
    if (nick.empty() || nick.size() > 9)
        return false;

    // Check first character
    if (!(isLetter(nick[0]) || isSpecial(nick[0])))
        return false;

    // Check next characters
    for (size_t i = 1; i < nick.size(); ++i) {
        char c = nick[i];
        if (!(isLetter(c) || isDigit(c) || isSpecial(c) || c == '-'))
            return false;
    }

    return true;
}

std::vector<std::string> splitString(const std::string &str, const std::string &delimiter) {
    if (delimiter.empty()) {
        return std::vector<std::string>(1, str);
    }

    std::vector<std::string> ret;
    std::string strCopy(str);
    size_t pos = 0;
    std::string current;

    while ((pos = strCopy.find(delimiter)) != std::string::npos) {
        current = strCopy.substr(0, pos);
        ret.push_back(current);
        strCopy.erase(0, pos + delimiter.length());
    }
    ret.push_back(strCopy);

    return ret;
}
