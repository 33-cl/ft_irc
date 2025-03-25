#pragma once

// #include "../messages.hpp"

#include "../client/Client.hpp"

#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>

class Client;


class Channel
{
    private:
        std::string         name;
        std::vector<Client> clients; // Stocking clients ids

    public:
        Channel();
        Channel(std::string name, const Client& client);
        ~Channel();

        void    add_client(Client new_client);
		bool	hasClient(int fd) const;
		void	removeClient(int fd);

        void    broadcast(const std::string& str);

        std::vector<Client>    get_clients() const;  

        friend class Server;
        friend class Client;
        friend class Command;
        friend class Join;
};
