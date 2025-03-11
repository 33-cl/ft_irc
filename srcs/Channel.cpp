#include "../includes/Channel.hpp"

Channel::Channel(std::string name, int client_fd)
{
    this->name = name;
    clients.push_back(client_fd);
}

Channel::~Channel()
{
}