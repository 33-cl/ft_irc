#pragma once

#include <iostream>
#include <utility>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <poll.h>
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
        void    start();
        void    handle_client(int client_fd);
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

    // Creer la socket
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
        throw std::runtime_error("socket() error");

    // Lier la socket a l'adresse et au port
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;           // Using ipv4
    server_addr.sin_addr.s_addr = INADDR_ANY;   // Accept connexions on every available network interface
    server_addr.sin_port = htons(_port);        // Converting port 
    if (bind(_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        throw std::runtime_error("bind() error");

    // Mettre en mode ecoute
    if (listen(_fd, 5) == -1)
        throw std::runtime_error("listen() error");

    std::cout << "Server initialized on port " << _port << std::endl;
}

void    Server::handle_client(int client_fd)
{
    char buffer[1024];
    while (true)
    {
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            std::cout << "Message reçu: " << buffer << std::endl;
        }
        else if (bytes_received == 0)
            break;
        else
        {
            std::cout << "else\n";
        }
    }
}

void    Server::start()
{
    std::vector<pollfd> fds;
    fds.push_back({ _fd, POLLIN, 0 });

    while (true)
    {
        int nb_fds = poll(fds.data(), fds.size(), -1);
        if (nb_fds == -1)
            break;
        
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == _fd)
                {
                    // Nouvelle connexion
                    sockaddr_in client_addr;
                    socklen_t   client_len = sizeof(client_addr);
                    int         client_fd;
                    
                    client_fd = accept(_fd, (sockaddr *)&client_addr, &client_len);
                    if (client_fd == -1)
                        continue;

                    std::cout << "Client connecte\n";
                    fds.push_back({ client_fd, POLLIN, 0 });
                }
                else
                {
                    // Message d'un client existant
                    char    buffer[1024];
                    ssize_t bytes_received = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

                    if (bytes_received > 0)
                    {
                        // Afficher le message recu
                        buffer[bytes_received] = '\0';
                        std::cout << "Message recu: " << buffer << std::endl;
                    }
                    else
                    {
                        // Supprimer le client deconnecte
                        std::cout << "Client deconnecte\n";
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;
                    }
                }
            }
        }
    }
    
}

void    Server::infos()
{
    std::cout << "port     : " << _port     << std::endl
              << "password : " << _password << std::endl;
}
