#include "Channel.hpp"

std::vector<int>    Channel::get_clients() const
{
    return this->clients;
}

void    Channel::add_client(int fd)
{
    clients.push_back(fd);
}