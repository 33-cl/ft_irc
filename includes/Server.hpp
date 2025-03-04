#pragma once

#include <iostream>
#include <utility>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

class Server
{
    private:
        std::string _password;
        int         _port;
        int         _fd;

    public:
        Server();
        ~Server();
        
        void    initialize(int argc, char **argv);
        void    infos();
};

Server::Server() : _password(""), _port(0) {}

Server::~Server() 
{
    close(_fd);
}

void    Server::initialize(int argc, char **argv)
{
    if (argc != 3)
        throw std::invalid_argument("Invalid nb of args\nusage: ./ircserv <port> <password>");
    
    std::string port_str = argv[1];
    if (port_str.find_first_not_of("0123456789") != std::string::npos)
        throw std::invalid_argument("Port must be numeric");

    _port = atoi(port_str.c_str());
    if (_port <= 0 || _port > 65535)
        throw std::invalid_argument("Port must be between 1 and 65535");

    _password = argv[2];

    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
        throw std::runtime_error("socket() error");

    // Structure stocking the informations of the server's address
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;           // Using ipv4
    server_addr.sin_addr.s_addr = INADDR_ANY;   // Accept connexions on every available network interface
    server_addr.sin_port = htons(_port);        // Converting port 

    // Linking the socket with the address and port
    if (bind(_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        throw std::runtime_error("bind() error");

    // Put the socket in listen mode (waiting list size 5)
    if (listen(_fd, 5) == -1)
        throw std::runtime_error("listen() error");
}

void    Server::infos()
{
    std::cout << "port     : " << _port     << std::endl
              << "password : " << _password << std::endl;
}