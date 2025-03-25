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

void    Channel::add_client(int fd)
{
    clients.push_back(fd);
}

void    Channel::broadcast(const std::string& str)
{
	(void)str;
	for (size_t i = 0; i < clients.size(); i++)
	{
		std::string message = str + "\r\n";
		ssize_t bytes_sent = send(clients[i], message.c_str(), message.length(), 0);
		if (bytes_sent == -1) {
			std::cerr << "Error while sending data" << std::endl;
		}
	}
	
}

std::vector<int>    Channel::get_clients() const
{
    return this->clients;
}