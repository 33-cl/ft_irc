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

bool	Channel::hasClient(int fd) const
{
	//A VERIFIER
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i] == fd)
			return true;
	}
	return false;
}