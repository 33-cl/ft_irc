#pragma once

#include <vector>
#include <string>

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

        std::vector<int>    get_clients() const;  

        friend class Server;
        friend class Client;
        friend class Command;
        friend class Join;
};
