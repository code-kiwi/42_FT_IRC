/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:08:42 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/01 08:33:23 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "helpers.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << RED << "Usage: ./ircserv [port] [password]" << WHITE << std::endl;
        return EXIT_FAILURE;
    }

    // Input validation
    int port;
    std::string password;
    try {
        port = stringToInt(argv[1]);
        password = argv[2];
        validatePort(port);
        validatePassword(password);
    } catch (const std::exception &e) {
        std::cerr << RED << "Error : " << e.what() << WHITE << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "ft_irc - port: " << port << " - password: " << password << std::endl;
    return EXIT_SUCCESS;
}
