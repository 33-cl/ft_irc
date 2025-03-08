#pragma once

#include "Client.hpp"

#include <iostream>
#include <utility>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <map>
#include <poll.h>
#include <arpa/inet.h>

class Server
{
    private:
        std::string              _password;
        int                      _port;
        int                      _fd;
        std::map<int, Client>    _clients;

    public:
        Server();
        ~Server();
        
        void    initialize(int argc, char **argv);
        void    start();
        void    new_client(std::vector<pollfd> &fds);
        void    process_client_data(std::vector<pollfd> &fds, int i);
        void    process_input(char *input, Client &client);

        void    infos();
};

