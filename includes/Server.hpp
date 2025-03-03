#pragma once

#include <iostream>
#include <utility>
#include <cstdlib>

class Server
{
    private:
        std::string _password;
        int         _port;

    public:
        Server();
        ~Server();
        
        void    initialize(int argc, char **argv);
        void    infos();
};

Server::Server() : _password(""), _port(0) {}

Server::~Server() {}

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
}

void    Server::infos()
{
    std::cout << "port     : " << _port     << std::endl
              << "password : " << _password << std::endl;
}