#pragma once

// #include "../messages.hpp"

#include <iostream>
#include <utility>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <map>
#include <poll.h>
#include <arpa/inet.h>
#include <string.h>

class Channel
{
    private:
        std::string         name;
        std::vector<int>    clients; // Stocking clients ids
    public:
        Channel();
        Channel(std::string name, int client_fd);
        ~Channel();

        void    add_client(int fd);
		bool	hasClient(int fd) const;

        void    broadcast(const std::string& str);

        std::vector<int>    get_clients() const;  

        friend class Server;
        friend class Client;
        friend class Command;
        friend class Join;
};
