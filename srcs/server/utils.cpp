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
                channel.broadcast(message, client);
			channel.removeClient(client.socket.fd);
			channel.removeInvite(client);
			channel.removeOperator(client);
            
            // Check if channel is empty after removing the client
            if (channel.clients.size() == 0)
                channelsToRemove.push_back(it->first);
		}
	}
    
    // Remove empty channels
    for (size_t i = 0; i < channelsToRemove.size(); ++i)
    {
        this->_channels.erase(channelsToRemove[i]);
    }
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
        // Vérifier si ':' est présent dans le segment courant
        size_t colon_pos = str.find(':', start);
        if (colon_pos != std::string::npos && colon_pos < end) 
        {
            // Si ':' est trouvé avant le prochain délimiteur, tout mettre dans le dernier token
            colon_found = true;
            break;
        }

        std::string token = str.substr(start, end - start);
        tokens.push_back(token);
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    // Gérer le reste de la chaîne (soit après le dernier délimiteur, soit après ':')
    if (start < str.length()) 
    {
        std::string remaining = str.substr(start);
        
        // Suppression des "\r\n" à la fin
        while (!remaining.empty() && (remaining[remaining.size()-1] == '\r' || remaining[remaining.size()-1] == '\n')) 
        {
            remaining.erase(remaining.size()-1);
        }
        
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
    if (tab.empty()) {
        return "";
    }

    std::string result;
    bool has_colon_token = false;

    // On commence à l'index 1 pour sauter la commande (ex: "JOIN")
    for (size_t i = 1; i < tab.size(); ++i) {
        if (!tab[i].empty() && tab[i][0] == ':') {
            // Token commençant par ':' - on l'ajoute et on ignore le reste
            if (!result.empty()) {
                result += ' ';
            }
            result += tab[i];
            has_colon_token = true;
            break;
        }

        if (i != 1) {  // Pas d'espace avant le premier argument
            result += ' ';
        }
        result += tab[i];
    }

    // Gestion des tokens avec ':' s'ils n'ont pas été traités
    if (!has_colon_token) {
        for (size_t i = 1; i < tab.size(); ++i) {
            if (!tab[i].empty() && tab[i][0] == ':') {
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
    if (input[input.size() - 1] == '\n') // With some clients, the input be followed by '\n'
        input.erase(input.size() - 1);   // so we remove it the handle the inputs equally

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
            
            // Affichage des membres du canal (clients)
            std::cout << "    Clients: ";
            for (size_t i = 0; i < it->second.clients.size(); i++) {
                // On utilise _clients pour afficher le nickname associé au fd
                std::cout << _clients.at(it->second.clients[i].socket.fd).nickname << "|";
            }
            std::cout << std::endl;
            
            // Affichage des opérateurs
            std::cout << "    Operators: ";
            if (it->second.operators.empty())
                std::cout << "None";
            else {
                for (size_t i = 0; i < it->second.operators.size(); i++) {
                    std::cout << it->second.operators[i].nickname << "|";
                }
            }
            std::cout << std::endl;
            
            // Affichage des invités
            std::cout << "    Invites: ";
            if (it->second.invites.empty())
                std::cout << "None";
            else {
                for (size_t i = 0; i < it->second.invites.size(); i++) {
                    std::cout << it->second.invites[i].nickname << "|";
                }
            }
            std::cout << std::endl;
            
            // Optionnel: Affichage des modes
            std::cout << "    Modes: " << it->second.getModesString() << std::endl;
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