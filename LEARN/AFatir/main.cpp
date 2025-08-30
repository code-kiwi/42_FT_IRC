#include <string>
#include <cstring>
#include <iostream>
#include <vector> // for vectors
#include <sys/socket.h> // for socket()
#include <sys/types.h> // for socket()
#include <netinet/in.h> // for sockaddr_in
#include <fcntl.h> // for fcntl()
#include <unistd.h> // for close()
#include <arpa/inet.h> // for inet_ntoa()
#include <poll.h> // for poll()
#include <csignal> // for signal()

/* ---------- */
#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

/* ---------- */

/* Client class */
class Client {
    public:
        Client(void) {}
        int getFd(void) const { return this->_fd; }
        const std::string& getIpAddress(void) const { return this->_ipAddress; }
        void setFd(int fd) { this->_fd = fd; }
        void setIpAddress(const std::string& ipAddress) { this->_ipAddress = ipAddress; }

    private:
        int _fd;
        std::string _ipAddress;
};

/* Server class */
class Server {
    public:
        Server(void) { this->_socketFd = -1; };

        void serverInit(void) { // server intialization
            this->_port = 4444;
            this->serverSocket(); // Creates the server socket

            std::cout << GRE << "Server <" << this->_socketFd << "> Connected" << WHI << std::endl;
	        std::cout << "Waiting to accept a connection..." << std::endl;

            // Server's loop until signal handling
            int returned;
            while (!Server::signal) {
                returned = poll(&(this->_fds[0]), this->_fds.size(), -1);
                if (returned == -1 && !Server::signal) {
                    throw std::runtime_error("The call to poll() failed");
                }

                // Checks all file descriptors
                for (size_t i = 0; i < this->_fds.size(); i++) {
                    // Checks if there is data to read
                    if (this->_fds[i].revents & POLLIN) {
                        if (this->_fds[i].fd == this->_socketFd) {
                            this->acceptNewClient();
                        } else {
                            receiveNewData(this->_fds[i].fd);
                        }
                    }
                }
            }
            closeFds();
        }

        void serverSocket(void) { // server socket creation
            struct sockaddr_in address;
            struct pollfd newPoll;
            int returned = 0;

            address.sin_family = AF_INET; // Set the address family to ipv4
            address.sin_port = htons(this->_port); // Converts the port to network byte order (big endian)
            address.sin_addr.s_addr = INADDR_ANY; // Sets the address to any local machine address

            this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);
            if (this->_socketFd == -1) { // Checks if the socket creation worked
                throw std::runtime_error("Failed to create the server socket");
            }

            // Set the socket option SO_REUSEADDR to reuse the address (avoids the "address already in use error with bind")
            int en = 1;
            returned = setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en));
            if (returned == -1) {
                throw std::runtime_error("Failed to set option SO_REUSEADDR to the server socket");
            }

            // Set the socket option O_NONBLOCK  for non-blocking socket (even blocking functions will not block the socket, an error will be returned instead)
            returned = fcntl(this->_socketFd, F_SETFL, O_NONBLOCK);
            if (returned == -1) {
                throw std::runtime_error("Failed to set option O_NONBLOCK on the server socket");
            }

            // Bind the socket to the address
            returned = bind(this->_socketFd, (struct sockaddr *) &address, sizeof(address));
            if (returned == -1) {
                throw std::runtime_error("Failed to bind the server socket");
            }

            // Listen for incoming connections and making the socket a passive one
            returned = listen(this->_socketFd, SOMAXCONN);
            if (returned == -1) {
                throw std::runtime_error("Failed to listen() on the server socket");
            }

            newPoll.fd = this->_socketFd; // Adds the server socket to pollfd
            newPoll.events = POLLIN; // set the event POLLIN for reading data
            newPoll.revents = 0; // Sets the revents to 0 (to clear the pollfd read events)
            this->_fds.push_back(newPoll);

            // Prints the IP on which the server is listening
            struct sockaddr_in sin;
            socklen_t len = sizeof(sin);

            returned = getsockname(this->_socketFd, (struct sockaddr *) &sin, &len);
            if (returned == -1) {
                perror("getsockname");
                return ;
            }
            std::cout << "Server bound on IP " << inet_ntoa(sin.sin_addr) << " and port " << ntohs(sin.sin_port) << std::endl;
        }

        void acceptNewClient(void) { // accept new client
            Client client;
            struct sockaddr_in clientAddress;
            struct pollfd newPoll;
            socklen_t len = sizeof(clientAddress);
            int returned = 0;

            // Accept new client
            int incomingFd = accept(this->_socketFd, (sockaddr *) &(clientAddress), &len);
            if (incomingFd == -1) {
                std::cout << "Call to accept() failed" << std::endl;
                return;
            }

            // Sets the socket option O_NONBLOCK for non-blocking socket
            returned = fcntl(incomingFd, F_SETFL, O_NONBLOCK);
            if (returned == -1) {
                std::cout << "Call to fcntl() failed" << std::endl;
                return;
            }

            newPoll.fd = incomingFd; // Adds the client's file descriptor
            newPoll.events = POLLIN; // Set the event to POLLIN for reading data
            newPoll.revents = 0; // Resets the revents value

            client.setFd(incomingFd); // Sets the client file descriptor
            client.setIpAddress(inet_ntoa(clientAddress.sin_addr)); // Converts the IP address and saves it
            this->_clients.push_back(client); // Adds the client to the server's vector of clients
            this->_fds.push_back(newPoll);

            std::cout << GRE << "Client <" << incomingFd << " - IP : " << client.getIpAddress() << "> Connected" << WHI << std::endl;
        }

        void receiveNewData(int fd) { // receive new data from a registered client
            char buffer[1024]; // buffer for received data
            memset(buffer, 0, sizeof(buffer)); // Clears the buffer

            ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0); // Receive the data

            if (bytes <= 0) {
                // Client disconnection
                std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
                this->clearClients(fd); // Removes the client from saved ones
                close(fd); // closes the client's socket
            } else {
                // Prints the received data
                buffer[bytes] = '\0';
                std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buffer << std::endl;
            }
        }

        static void signalHandler(int signum) { // signal handler
            std::cout << std::endl << "Signal received : " << signum << std::endl;
            Server::signal = true;
        }

        void closeFds(void) { // close all fds
            // Close all the clients
            for (size_t i = 0; i < this->_clients.size(); i++) {
                close(this->_clients[i].getFd());
                std::cout << RED << "Client <" << this->_clients[i].getFd() << "> Disconnected" << WHI << std::endl;
            }

            // Close the server socket
            if (this->_socketFd != -1) {
                close(this->_socketFd);
                std::cout << RED << "Server <" << this->_socketFd << "> Disconnected" << WHI << std::endl;
            }
        }

        void clearClients(int fd) { // clear clients
            // Remove the client from the pollfds
            for (size_t i = 0; i < this->_fds.size(); i++) {
                if (this->_fds[i].fd == fd) {
                    this->_fds.erase(this->_fds.begin() + i);
                    break;
                }
            }
            
            // Remove the client from the client's vector
            for (size_t i = 0; i < this->_clients.size(); i++) {
                if (this->_clients[i].getFd() == fd) {
                    this->_clients.erase(this->_clients.begin() + i);
                    break;
                }
            }
        }

    private:
        int _port; // server's port
        int _socketFd; // server socket fd
        std::vector<Client> _clients; // for storing clients
        std::vector<struct pollfd> _fds; // for storing poll fds (for poll() function calls)

        static bool signal; // for handling signals
};

bool Server::signal = false;

int main(void) {
    Server server;

    std::cout << "----------SERVER----------" << std::endl;
    try {
        signal(SIGINT, Server::signalHandler); // Catches the signal (ctrl + c)
        signal(SIGQUIT, Server::signalHandler); // Catches the signal (ctrl + \)
        server.serverInit(); // Initializes the server
    } catch (const std::exception& e) {
        server.closeFds();
        std::cerr << e.what() << std::endl;
    }
    std::cout << "The server closed !" << std::endl;
    return 0;
}