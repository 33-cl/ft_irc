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

void    Channel::add_client(const Client& new_client)
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

void    Channel::broadcastEveryone(const std::string& str, const Client& src)
{
	(void)src;
	for (size_t i = 0; i < clients.size(); i++)
	{
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
	std::cout << "addops\n";

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

bool Channel::has_operator() const
{
    return !operators.empty();
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
	for (std::map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); ++it)
	{
		if (it->second)
			ss << it->first;
	}

	std::map<char, bool>::const_iterator it_l = modes.find('l');
	if (it_l != modes.end() && it_l->second && usersLimit > 0)
		ss << " " << usersLimit;

	std::map<char, bool>::const_iterator it_k = modes.find('k');
	if (it_k != modes.end() && it_k->second && !password.empty())
		ss << " " << password;

	return ss.str();
}

int		Channel::getClientCount() const
{
	return clients.size();
}

const std::string&  Channel::getLastAppliedModes() const
{
	return _lastAppliedModes;
}

const std::vector<std::string>& Channel::getLastAppliedParams() const
{
	return _lastAppliedParams;
}


bool Channel::changeMode(const std::string &modeChanges, const std::vector<std::string>& modeParams,Client& client, Server& server)
{
	bool adding = true;
	size_t paramIndex = 0;
	std::vector<std::string> errors;
	std::string appliedModes;
	std::vector<std::string> appliedParams;

	for (size_t i = 0; i < modeChanges.size(); ++i)
	{
		char c = modeChanges[i];

		if (c == '+' || c == '-')
		{
			adding = (c == '+');
			appliedModes += c;
			continue;
		}
		if (c == 'i' || c == 't')
		{
			modes[c] = adding;
			appliedModes += c;
			continue;
		}
		if (c == 'l')
		{
			if (adding)
			{
				if (paramIndex >= modeParams.size())
				{
					errors.push_back(ERR_NEEDMOREPARAMS(client.nickname, "MODE +l"));
				}
				else
				{
					const std::string limitStr = modeParams[paramIndex++];
					bool valid = true;
					for (size_t j = 0; j < limitStr.size(); ++j)
					{
						if (!isdigit(limitStr[j]))
						{
							valid = false;
							break;
						}
					}
					if (!valid)
						errors.push_back(ERR_INVALIDUSERLIMIT(client.nickname));
					else
					{
						int limit = atoi(limitStr.c_str());
						if (limit <= 0 || limit > 10000)
							errors.push_back(ERR_INVALIDUSERLIMIT(client.nickname));
						else
						{
							modes['l'] = true;
							usersLimit = limit;
							appliedModes   += 'l';
							appliedParams.push_back(limitStr);
						}
					}
				}
			}
			else
			{
				modes['l']     = false;
				usersLimit     = 0;
				appliedModes  += 'l';
			}
			continue;
		}

		if (c == 'k')
		{
			if (adding)
			{
				if (paramIndex >= modeParams.size())
					errors.push_back(ERR_NEEDMOREPARAMS(client.nickname, "MODE +k"));
				else
				{
					const std::string keyParam = modeParams[paramIndex++];
					bool valid = !keyParam.empty();
					for (size_t j = 0; j < keyParam.size(); ++j)
					{
						if (keyParam[j] < 32 || keyParam[j] > 126)
						{
							valid = false;
							break;
						}
					}
					if (!valid)
						errors.push_back(ERR_EMPTYKEY(client.nickname));
					else
					{
						modes['k'] = true;
						password   = keyParam;
						appliedModes   += 'k';
						appliedParams.push_back(keyParam);
					}
				}
			}
			else
			{
				modes['k'] = false;
				password   = "";
				appliedModes   += 'k';
			}
			continue;
		}

		if (c == 'o')
		{
			std::cout << "ooooo\n";
			if (paramIndex >= modeParams.size())
				errors.push_back(ERR_NEEDMOREPARAMS(client.nickname, adding ? "MODE +o" : "MODE -o"));
			else
			{
				const std::string nick = modeParams[paramIndex++];
				bool found = false;

				std::vector<Client>& list = (adding ? clients : operators);
				for (std::vector<Client>::iterator it = list.begin(); it != list.end(); ++it)
				{
					if (it->nickname == nick)
					{
						found = true;
						if (adding)
						{
							if (isOperator(*it))
								errors.push_back(ERR_ALREADYOP(client.nickname, nick));
							else
							{
								addOperator(*it);
								appliedModes   += 'o';
								appliedParams.push_back(nick);
							}
						}
						else
						{
							if (!isOperator(*it))
								errors.push_back(ERR_NOTANOPERATOR(client.nickname, nick));
							else
							{
								removeOperator(*it);
								appliedModes   += 'o';
                        		appliedParams.push_back(nick);
							}
						}
						break;
					}
				}
				if (!found)
					errors.push_back(ERR_NOSUCHNICK(client.nickname, nick));
			}
			continue;
		}
		errors.push_back(ERR_UNKNOWNMODE(client.nickname, std::string(1, c)));
	}

	while (paramIndex < modeParams.size())
		errors.push_back(ERR_UNKNOWNMODE(client.nickname, "Unexpected parameter: " + modeParams[paramIndex++]));

	for (std::vector<std::string>::const_iterator eit = errors.begin(); eit != errors.end(); ++eit)
		client.write(":" + server._name + " " + *eit);

	_lastAppliedModes  = appliedModes;
	_lastAppliedParams = appliedParams;

	return errors.empty();
}
