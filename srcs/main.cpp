/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:08:42 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/16 18:33:30 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "config.hpp"
#include "helpers.hpp"

#include <csignal>
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

    // Server creation
    std::cout << "ft_irc - port: " << port << " - password: " << password << std::endl;
    Server server(port, SERVER_NAME, password);
    try {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        server.init();
    } catch (const std::exception &e) {
        server.closeFds();
        std::cerr << "Server crash: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "The server closed !" << std::endl;
    return EXIT_SUCCESS;
}
