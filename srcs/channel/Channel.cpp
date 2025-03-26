#include "Channel.hpp"

Channel::Channel() : name("") {}

Channel::Channel(std::string name, const Client& client)
{
    this->name = name;
    clients.push_back(client);
}

Channel::~Channel()
{
}

bool	Channel::hasClient(int fd) const
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].socket.fd == fd)
			return true;
	}
	return false;
}

void    Channel::add_client(Client new_client)
{
    clients.push_back(new_client);
}

void    Channel::broadcast(const std::string& str, const Client& src)
{
	(void)src;
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (src.socket.fd != clients[i].socket.fd)
			clients[i].write(str);
	}	
}

std::vector<Client>    Channel::get_clients() const
{
    return this->clients;
}

void Channel::removeClient(int fd)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->socket.fd == fd)
		{
			clients.erase(it);
			break;
		}
	}
}