#include "Channel.hpp"

Channel::Channel() : name("") {}

Channel::Channel(std::string name, int client_fd)
{
    this->name = name;
    clients.push_back(client_fd);
}

Channel::~Channel()
{
}

std::vector<int>    Channel::get_clients() const
{
    return this->clients;
}

void    Channel::add_client(int fd)
{
    clients.push_back(fd);
}