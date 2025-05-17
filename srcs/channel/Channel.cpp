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

// bool Channel::changeMode(const std::string &modeChanges, const std::vector<std::string>& modeParams, Client& client, Server& server)
// {
// 	bool adding = true;
// 	size_t paramIndex = 0;
// 	std::vector<std::string> errors;

// 	for (size_t i = 0; i < modeChanges.size(); ++i)
// 	{
// 		char c = modeChanges[i];

// 		if (c == '+')
// 			adding = true;
// 		else if (c == '-')
// 			adding = false;
// 		else
// 		{
// 			switch(c)
// 			{
// 				case 'i':
// 					modes['i'] = adding;
// 					break;

// 				case 't':
// 					modes['t'] = adding;
// 					break;

// 				case 'l':
// 					if (adding)
// 					{
// 						if (paramIndex >= modeParams.size())
// 					            errors.push_back(ERR_NEEDMOREPARAMS(client.nickname, "MODE +l"));
// 						else
// 						{
// 							const std::string& limitStr = modeParams[paramIndex];
// 							for (size_t j = 0; j < limitStr.size(); ++j) {
// 								if (!isdigit(limitStr[j])) {
// 									errors.push_back(ERR_INVALIDUSERLIMIT(client.nickname));
// 									++paramIndex;
// 									break;
// 								}
// 							}

// 							if (errors.empty()) {
// 								int limit = atoi(limitStr.c_str());
// 								if (limit <= 0 || limit > 10000)
// 									errors.push_back(ERR_INVALIDUSERLIMIT(client.nickname));
// 								else
// 								{
// 									usersLimit = limit;
// 									modes['l'] = true;
// 								}
// 								++paramIndex;
// 							}
// 						}
// 					}
// 					else
// 					{
// 						modes['l'] = false;
// 						usersLimit = 0;
// 					}
// 					break;

// 				case 'k':
// 					if (adding)
// 					{
// 						if (paramIndex >= modeParams.size())
// 							errors.push_back(ERR_NEEDMOREPARAMS(client.nickname, "MODE +k"));
// 						else
// 						{
// 							std::string keyParam = modeParams[paramIndex];
// 							if (keyParam.empty() || keyParam[0] == ' ')
// 								errors.push_back(ERR_EMPTYKEY(client.nickname));
// 							else
// 							{
// 								bool valid = true;
// 								for (size_t j = 0; j < keyParam.size(); ++j) {
// 									if (keyParam[j] < 32 || keyParam[j] > 126) {
// 										valid = false;
// 										break;
// 									}
// 								}
								
// 								if (!valid) {
// 									errors.push_back(ERR_EMPTYKEY(client.nickname));
// 								} else {
// 									password = keyParam;
// 									modes['k'] = true;
// 								}
// 							}
// 							++paramIndex;
// 						}
// 					}
// 					else
// 					{
// 						modes['k'] = false;
// 						password = "";
// 					}
// 					break;

// 				case 'o':
// 					if (adding)
// 					{
// 						if (paramIndex >= modeParams.size())
// 							errors.push_back(ERR_NEEDMOREPARAMS(client.nickname, "MODE +o"));
// 						else
// 						{
// 							std::string nickToOp = modeParams[paramIndex];
// 							bool found = false;
// 							for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
// 							{
// 								if (it->nickname == nickToOp)
// 								{
// 									if (!isOperator(*it))
// 									{
// 										addOperator(*it);
// 										found = true;
// 										break;
// 									}
// 									else
// 									{
// 										errors.push_back(ERR_ALREADYOP(client.nickname, nickToOp));
// 										found = true;
// 									}
// 								}
// 							}
// 							if (!found)
// 							{
// 								errors.push_back(ERR_NOSUCHNICK(client.nickname, nickToOp));
// 								found = true;
// 							}
// 							++paramIndex;
// 						}
// 					}
// 					else
// 					{
// 						if (paramIndex >= modeParams.size())
// 							errors.push_back(ERR_NEEDMOREPARAMS(client.nickname, "MODE -o"));
// 						else
// 						{
// 							std::string nickToDeop = modeParams[paramIndex];
// 							bool found = false;
// 							for (std::vector<Client>::iterator it = operators.begin(); it != operators.end(); ++it)
// 							{
// 								if (it->nickname == nickToDeop)
// 								{
// 									if (isOperator(*it))
// 									{
// 										removeOperator(*it);
// 										found = true;
// 										break;
// 									}
// 									else
// 									{
// 										errors.push_back(ERR_NOTANOPERATOR(client.nickname, nickToDeop));
// 										found = true;
// 									}
// 								}
// 							}
// 							if (!found)
// 								errors.push_back(ERR_NOSUCHNICK(client.nickname, nickToDeop));
// 							++paramIndex;
// 						}
// 					}
// 					break;

// 				default:
// 					errors.push_back(ERR_UNKNOWNMODE(client.nickname, std::string(1, c)));
// 					break;
// 			}
// 		}
// 	}
// 	for (std::vector<std::string>::const_iterator it = errors.begin(); it != errors.end(); ++it)
// 	{
// 		client.write(":" + server._name + " " + *it);
// 	}

// 	return errors.empty();
// }


bool Channel::changeMode(const std::string &modeChanges,
                         const std::vector<std::string>& modeParams,
                         Client& client, Server& server)
{
    bool adding = true;
    size_t paramIndex = 0;
    std::vector<std::string> errors;

    // Pour la passe de collecte
    std::string appliedModes;
    std::vector<std::string> appliedParams;

    // --- Passe 1 : lecture & validation ---
    for (size_t i = 0; i < modeChanges.size(); ++i)
    {
        char c = modeChanges[i];
        if (c == '+' || c == '-')
        {
            adding = (c == '+');
            appliedModes += c;
            continue;
        }

        // 1) Est-ce un mode connu ?
        if (c != 'i' && c != 't' && c != 'l' && c != 'k' && c != 'o')
        {
            errors.push_back(ERR_UNKNOWNMODE(client.nickname, std::string(1, c)));
            continue;
        }

        // 2) Modes sans param : i, t
        if (c == 'i' || c == 't')
        {
            modes[c] = adding;
            appliedModes += c;
            continue;
        }

        // 3) Modes avec param (l, k, o)
        if (paramIndex >= modeParams.size())
        {
            // Pas assez de paramètres
            std::string m = std::string("MODE ") + (adding ? "+" : "-") + c;
            errors.push_back(ERR_NEEDMOREPARAMS(client.nickname, m));
            continue;
        }

        const std::string param = modeParams[paramIndex++];
        bool valid = true;

        if (c == 'l')
        {
            // numeric only
            for (size_t j = 0; j < param.size(); ++j)
                if (!isdigit(param[j])) { valid = false; break; }
            if (valid)
            {
                int limit = atoi(param.c_str());
                valid = (limit > 0 && limit <= 10000);
            }
            if (!valid)
                errors.push_back(ERR_INVALIDUSERLIMIT(client.nickname));
            else
            {
                modes['l'] = adding;
                usersLimit = atoi(param.c_str());
                appliedModes += 'l';
                appliedParams.push_back(param);
            }
        }
        else if (c == 'k')
        {
            // printable only
            for (size_t j = 0; j < param.size(); ++j)
                if (param[j] < 32 || param[j] > 126) { valid = false; break; }
            if (!valid || param.empty())
                errors.push_back(ERR_EMPTYKEY(client.nickname));
            else
            {
                modes['k'] = adding;
                password = param;
                appliedModes += 'k';
                appliedParams.push_back(param);
            }
        }
        else if (c == 'o')
        {
            // op/deop (toujours param)
            bool found = false;
            std::vector<Client> & list = (adding ? clients : operators);
            for (std::vector<Client>::iterator it = list.begin(); it != list.end(); ++it)
            {
                if (it->nickname == param)
                {
                    found = true;
                    if (adding)      addOperator(*it);
                    else if (isOperator(*it)) removeOperator(*it);
                    else          errors.push_back(ERR_NOTANOPERATOR(client.nickname, param));
                    break;
                }
            }
            if (!found)
                errors.push_back(ERR_NOSUCHNICK(client.nickname, param));
            else
            {
                appliedModes += 'o';
                appliedParams.push_back(param);
            }
        }
    }

    // --- Passe 1b : tous les params doivent être consommés ---
    while (paramIndex < modeParams.size())
    {
        errors.push_back(ERR_UNKNOWNMODE(
            client.nickname,
            "Unexpected parameter: " + modeParams[paramIndex++]));
    }

    // Conserve pour le broadcast
    _lastAppliedModes  = appliedModes;
    _lastAppliedParams = appliedParams;

    // Envoie des erreurs
    for (std::vector<std::string>::const_iterator it = errors.begin();
         it != errors.end(); ++it)
    {
        client.write(":" + server._name + " " + *it);
    }

    return errors.empty();
}
