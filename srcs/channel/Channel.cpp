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

void    Channel::broadcast(const std::string& str)
{
	(void)str;
	for (size_t i = 0; i < clients.size(); i++)
	{
		std::string message = str + "\r\n";
		ssize_t bytes_sent = send(clients[i].socket.fd, message.c_str(), message.length(), 0);
		if (bytes_sent == -1) {
			std::cerr << "Error while sending data" << std::endl;
		}
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