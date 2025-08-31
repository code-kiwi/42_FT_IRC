/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 18:34:38 by mhotting          #+#    #+#             */
/*   Updated: 2025/08/31 18:51:07 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.hpp"
#include "config.hpp"

#include <sstream>
#include <stdexcept>

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
