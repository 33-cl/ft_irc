#pragma once

#include <vector>
#include <string>

class Channel
{
    private:
        std::string         name;
        std::vector<int>    clients; // Stocking clients ids
    public:
        Channel(std::string name, int client_fd);
        ~Channel();
};

