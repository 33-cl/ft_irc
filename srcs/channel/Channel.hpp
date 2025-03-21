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


        std::vector<int>    get_clients() const;  
};
