#include "Server.hpp"

/*
    Returns a copy of the server itself
    Used for the signals, an alternative of using a global variable
*/

Server& Server::get_server()
{
    static Server serv;
    return  serv;
}

/*
    Returns a reference of the client by finding it via nickname
*/

Client& Server::find_client(const std::string& nickname)
{
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.nickname == nickname)
            return (it->second);
    }
    throw recoverable_error("Client not found");
}

/*
    Removes a client from the server
*/

void Server::remove_client(const Client& client, const std::string& message)
{
    std::vector<std::string> channelsToRemove;
    
    for (std::map<std::string, Channel>::iterator it = this->_channels.begin();
			it != this->_channels.end(); ++it)
	{
		Channel& channel = it->second;
		if (channel.hasClient(client.socket.fd))
		{
            if (!message.empty())
                channel.broadcastEveryone(message, client);
			channel.removeClient(client.socket.fd);
			channel.removeInvite(client);
			channel.removeOperator(client);
            
            if (channel.clients.size() == 0)
                channelsToRemove.push_back(it->first);
		}
	}
    
    for (size_t i = 0; i < channelsToRemove.size(); ++i)
        this->_channels.erase(channelsToRemove[i]);
}

void Server::destroy_channel(Channel& channel) 
{
    const std::string channel_name = channel.name;
    
    const std::string kick_msg = ":ircserv KICK " + channel_name +  
                               " * :No operators remaining\r\n";
    
    const std::string destroy_msg = ":ircserv NOTICE " + channel_name + 
                                  " :Channel being destroyed (no operators left)\r\n";
    
    const std::string kill_msg = ":ircserv KILL " + channel_name + 
                               " :No operators remaining (channel destroyed)\r\n";

    for (std::vector<Client>::iterator it = channel.clients.begin(); 
         it != channel.clients.end(); ) 
    {
        send(it->socket.fd, destroy_msg.c_str(), destroy_msg.length(), 0);
        send(it->socket.fd, kick_msg.c_str(), kick_msg.length(), 0);  
        send(it->socket.fd, kill_msg.c_str(), kill_msg.length(), 0);
          
        it = channel.clients.erase(it);
    }

    _channels.erase(channel_name);
}


/*
    Some sort of ft_split() for strings

    Also suppresses "\r\n" at the end of the string
*/

std::vector<std::string> split(const std::string& str, const std::string& delimiter) 
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    bool colon_found = false;

    while (end != std::string::npos && !colon_found) 
    {
        size_t colon_pos = str.find(':', start);
        if (colon_pos != std::string::npos && colon_pos < end) 
        {
            colon_found = true;
            break;
        }

        std::string token = str.substr(start, end - start);
        tokens.push_back(token);
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    if (start < str.length()) 
    {
        std::string remaining = str.substr(start);
        
        while (!remaining.empty() && (remaining[remaining.size()-1] == '\r' || remaining[remaining.size()-1] == '\n')) 
            remaining.erase(remaining.size()-1);
        
        tokens.push_back(remaining);
    }
    return tokens;
}

/*
    Split all whitespaces
*/

std::vector<std::string> split_white_spaces(const std::string& str)
{
	std::vector<std::string> tokens;
	size_t i = 0;
	while (i < str.size())
	{
		while (i < str.size() && std::isspace(static_cast<unsigned char>(str[i])))
			++i;
		if (i >= str.size())
			break;
		
		if (str[i] == ':')
		{
			tokens.push_back(str.substr(i));
			break;
		}
		
		size_t start = i;
		while (i < str.size() && !std::isspace(static_cast<unsigned char>(str[i])))
			++i;
		tokens.push_back(str.substr(start, i - start));
	}
	return tokens;
}

/*
    Reverse split for split_white_space function
*/

std::string unsplit(const std::vector<std::string>& tab) 
{
    if (tab.empty())
        return "";

    std::string result;
    bool has_colon_token = false;

    for (size_t i = 1; i < tab.size(); ++i)
	{
        if (!tab[i].empty() && tab[i][0] == ':')
		{
            if (!result.empty())
                result += ' ';
            result += tab[i];
            has_colon_token = true;
            break;
        }

        if (i != 1) 
            result += ' ';

        result += tab[i];
    }

    if (!has_colon_token) {
        for (size_t i = 1; i < tab.size(); ++i)
		{
            if (!tab[i].empty() && tab[i][0] == ':')
			{
                if (!result.empty()) {
                    result += ' ';
                }
                result += tab[i];
                has_colon_token = true;
            }
        }
    }
    return result;
}

/*
    Checks if the command given in "input" is "command"

    Also removes the "\r\n" at the end of the line
*/

bool check_command(std::string& input, const std::string& command) 
{
    if (input[input.size() - 1] == '\n')
        input.erase(input.size() - 1);

    if (input.size() < command.size() + 2 || input.compare(0, command.size() + 1, command + " ") != 0)
        return false;

    return true;
}

/*
    Returns a bool indicating if password is valid
*/

bool is_password_valid(const std::string& password) 
{
    if (password.length() < 4 || password.length() > 32) 
        return false;

    for (size_t i = 0; i < password.length(); ++i) 
    {
        char c = password[i];
        if (!isalnum(c) && 
            c != '-' && c != '_' && c != '.' && c != '!' && 
            c != '@' && c != '#' && c != '$' && c != '%' && 
            c != '^' && c != '&' && c != '*' && c != '?' && c != '+')
        {
            return false;
        }
    }

    return true;
}

/*
    Prints all the informations of the server

    Tool for debugging
*/

void Server::infos()
{
    std::cout << "     -------------------\n";

    // Server
    std::cout << "port     : " << _port     << std::endl
              << "password : " << _password << std::endl
              << "server fd: " << _fd       << std::endl;
    
    // Clients
    std::cout << "\nClients (" << _clients.size() << ") :" << std::endl;
    if (_clients.empty())
        std::cout << "  No clients connected" << std::endl;
    else
    {
        for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
        {
            std::cout << "  Client fd: " << it->first << std::endl
                      << "    Nickname: " << it->second.nickname << std::endl
                      << "    Username: " << it->second.username << std::endl
                      << "    Status  : " << it->second.status << std::endl;
        }
    }
    
    // Channels
    std::cout << "\nChannels (" << _channels.size() << ") :" << std::endl;
    if (_channels.empty())
    {
        std::cout << "  No channels created" << std::endl;
    }
    else
    {
        for (std::map<std::string, Channel>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
        {
            std::cout << "  Channel: " << it->first << std::endl;
            
            std::cout << "    Clients: ";
            for (size_t i = 0; i < it->second.clients.size(); i++) {
                std::cout << _clients.at(it->second.clients[i].socket.fd).nickname << "|";
            }
            std::cout << std::endl;
            
            std::cout << "    Operators: ";
            if (it->second.operators.empty())
                std::cout << "None";
            else {
                for (size_t i = 0; i < it->second.operators.size(); i++) {
                    std::cout << it->second.operators[i].nickname << "|";
                }
            }
            std::cout << std::endl;
            
            std::cout << "    Invites: ";
            if (it->second.invites.empty())
                std::cout << "None";
            else {
                for (size_t i = 0; i < it->second.invites.size(); i++) {
                    std::cout << it->second.invites[i].nickname << "|";
                }
            }
            std::cout << std::endl;
            
            std::cout << "    Modes: " << it->second.getModesString() << std::endl;

            std::cout << "    Topic: " << it->second.topic << std::endl;
        }
    }
}


int	Server::getFdByNickname(const std::string &nickname) const
{
	for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.nickname == nickname)
			return it->first;
	}
	return -1;
}

void Server::delete_clients() 
{
    for (std::map<int, Client>::iterator it = _clients.begin(); 
         it != _clients.end(); 
         ++it) {
        close(it->second.socket.fd);
    }
    _clients.clear();
}

void Server::broadcast_channel_lists() 
{
    for (std::map<std::string, Channel>::iterator chan_it = _channels.begin(); 
         chan_it != _channels.end(); ++chan_it) {
        
        Channel& channel = chan_it->second;
        
        for (std::vector<Client>::iterator member_it = channel.clients.begin();
             member_it != channel.clients.end(); ++member_it) {
            
            this->send_user_list(*member_it, channel);
        }
    }
}

void Server::send_user_list(Client& client, Channel& channel)
{
	std::string names_list = ":" + _name + " 353 " + client.nickname + " = " + channel.name + " :";
	bool first = true;

	for (std::vector<Client>::iterator member_it = channel.clients.begin();
		 member_it != channel.clients.end(); ++member_it)
	{
		if (!first)
			names_list += " ";
		first = false;

		bool is_operator = false;
		for (std::vector<Client>::iterator op_it = channel.operators.begin();
			 op_it != channel.operators.end(); ++op_it)
		{
			if (op_it->socket.fd == member_it->socket.fd)
			{
				is_operator = true;
				break;
			}
		}

		if (is_operator)
			names_list += "@";
		names_list += member_it->nickname;
	}

	client.write(names_list);
	client.write(":" + _name + " 366 " + client.nickname + " " + channel.name + " :End of /NAMES list");
}

void Server::remove_and_broadcast_list(const Client& client)
{
    std::vector<std::string> channels_to_update;
    
    // Parcourir tous les canaux pour trouver ceux où le client est présent
    for (std::map<std::string, Channel>::iterator chan_it = _channels.begin(); 
         chan_it != _channels.end(); ++chan_it) {
        
        Channel& channel = chan_it->second;
        
        // Vérifier si le client est dans ce canal
        if (channel.hasClient(client.socket.fd)) {
            channels_to_update.push_back(chan_it->first);
            
            // Supprimer le client du canal
            channel.removeClient(client.socket.fd);
            channel.removeInvite(client);
            channel.removeOperator(client);
        }
    }
    
    // Envoyer les listes à jour uniquement aux canaux concernés
    for (size_t i = 0; i < channels_to_update.size(); ++i) {
        if (_channels.find(channels_to_update[i]) != _channels.end()) {
            Channel& channel = _channels[channels_to_update[i]];
            
            for (std::vector<Client>::iterator member_it = channel.clients.begin();
                 member_it != channel.clients.end(); ++member_it) {
                
                this->send_user_list(*member_it, channel);
            }
        }
    }
}

/*
    Configure a socket to be non-blocking using fcntl
*/
bool Server::set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        return false;
    
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
        return false;
    
    return true;
}