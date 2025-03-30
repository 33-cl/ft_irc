#include "Channel.hpp"
#include "../messages.hpp"


Channel::Channel() : name(""), topic(""), invites(), operators(), modes(), usersLimit(0), password("")
{
	modes['i'] = false;
	modes['t'] = true;
	modes['k'] = false;
	modes['l'] = false;
}

Channel::Channel(std::string name, const Client& client)
{
    this->name = name;
	topic = "";
	invites.clear();
	operators.clear();
	modes.clear();
	usersLimit = 0;
	password = "";
	modes['i'] = false;
	modes['t'] = true;
	modes['k'] = false;
	modes['l'] = false;
    clients.push_back(client);
	addOperator(client);
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

void Channel::addInvite(const Client& client)
{
	if (!isInvited(client))
		invites.push_back(client);
}

void Channel::addOperator(const Client& client)
{
	if (!isOperator(client))
		operators.push_back(client);
}

void Channel::removeInvite(const Client& client)
{
	for (std::vector<Client>::iterator it = invites.begin(); it != invites.end(); ++it)
	{
		if (it->socket.fd == client.socket.fd)
		{
			invites.erase(it);
			break;
		}
	}
}


void Channel::removeOperator(const Client& client)
{
	for (std::vector<Client>::iterator it = operators.begin(); it != operators.end(); ++it)
	{
		if (it->socket.fd == client.socket.fd)
		{
			operators.erase(it);
			break;
		}
	}
}

bool Channel::isOperator(const Client& client) const
{
	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i].socket.fd == client.socket.fd)
			return true;
	}
	return false;
}

bool Channel::isInvited(const Client& client) const
{
	for (size_t i = 0; i < invites.size(); i++)
	{
		if (invites[i].socket.fd == client.socket.fd)
			return true;
	}
	return false;
}

std::string Channel::getModesString() const
{
	std::stringstream ss;

	ss << "+";
	//pou chaque mode actif on l'ajoute a la string
	for (std::map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); ++it)
	{
		if (it->second)
			ss << it->first;
	}

	// add users limit if 'l' mode is set
	std::map<char, bool>::const_iterator it_l = modes.find('l');
	if (it_l != modes.end() && it_l->second && usersLimit > 0)
		ss << " " << usersLimit;

	// add password if 'k' mode is set
	std::map<char, bool>::const_iterator it_k = modes.find('k');
	if (it_k != modes.end() && it_k->second && !password.empty())
		ss << " " << password;

	return ss.str();
}

bool Channel::changeMode(const std::string &modeChanges, const std::vector<std::string>& modeParams, Client& client)
{
	bool adding = true;
	size_t paramIndex = 0;


	for (size_t i = 0; i < modeChanges.size(); ++i)
	{
		char c = modeChanges[i];

		if (c == '+')
			adding = true;
		else if (c == '-')
			adding = false;
		else
		{
			switch(c)
			{
				case 'i':
					modes['i'] = adding;
					break;

				case 't':
					modes['t'] = adding;
					break;

				case 'l':
					if (adding)
					{
						if (paramIndex >= modeParams.size())
						{
							return false;
						}
						int limit = atoi(modeParams[paramIndex].c_str());
						if (limit <= 0 || limit > 10000)
						{
							return false;
						}
						usersLimit = limit;
						modes['l'] = true;
						++paramIndex;
					}
					else
					{
						modes['l'] = false;
						usersLimit = 0;
					}
					break;
					
				case 'k':
					if (adding)
					{
						if (paramIndex >= modeParams.size())
							return false;
						std::string keyParam = modeParams[paramIndex];
						
						if (keyParam.empty())
							return false;
						password = keyParam;
						modes['k'] = true;
						++paramIndex;
					}
					else
					{
						modes['k'] = false;
						password = "";
					}
					break;
			
				case 'o':
					if (adding)
					{
						if (paramIndex >= modeParams.size())
							return false;
						std::string nickToOp = modeParams[paramIndex];
						bool found = false;

						//is the client present in chan
						for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
						{
							if (it->nickname == nickToOp)
							{
								if (!isOperator(*it))
								{
									addOperator(*it);
									found = true;
									break;
								}
							}
						}
						if (!found)
   							throw recoverable_error(ERR_OPERATORNOTFOUND(client.nickname, nickToOp));
						++paramIndex;
					}
					else
					{
						if (paramIndex >= modeParams.size())
							return false;
						std::string nickToDeop = modeParams[paramIndex];
						bool found = false;
						for (std::vector<Client>::iterator it = operators.begin(); it != operators.end(); ++it)
						{
							if (it->nickname == nickToDeop)
							{
								removeOperator(*it);
								found = true;
								break;
							}
						}
						if (!found)
   							throw recoverable_error(ERR_OPERATORNOTFOUND(client.nickname, nickToDeop));
						++paramIndex;
					}
					break;

				default:
					return false;
			}
		}
	}
	return true;
}
