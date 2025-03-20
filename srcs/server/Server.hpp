#pragma once

#include "../client/Client.hpp"
#include "../channel/Channel.hpp"

#include <iostream>
#include <utility>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <map>
#include <poll.h>
#include <arpa/inet.h>
#include <string.h>

class Server
{
    private:
        std::string                     _password;
        int                             _port;
        int                             _fd;
        std::map<int, Client>           _clients;
        std::map<std::string, Channel>  _channels;

    public:
        Server();
        ~Server();

        void    initialize(int argc, char **argv);
        void    start();
        void    new_client(std::vector<pollfd>& fds);
        void    process_client_data(std::vector<pollfd>& fds, int i);
        void    process_input(std::string& input, Client& client);
        void    create_channel(const std::string& channel_name, Client& client);

        void    infos();
};

std::vector<std::string> split(const std::string& str, const std::string& delimiter = "\r\n");