#include "Server.hpp"

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
    Checks if the command given in "input" is "command"

    Also removes the "\r\n" at the end of the line
*/

bool check_command(std::string& input, const std::string& command) {
    if (input[input.size() - 1] == '\n') // With some clients, the input be followed by '\n'
        input.erase(input.size() - 1);   // so we remove it the handle the inputs equally

    if (input.size() < command.size() + 2 || input.compare(0, command.size() + 1, command + " ") != 0) {
        return false;
    }

    return true;
}

/*
    Prints all the informations of the server

    Tool for debugging
*/

void    Server::infos()
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
        std::cout << "  No channels created" << std::endl;
    else
    {
        for (std::map<std::string, Channel>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
        {
            std::cout << "  Channel: " << it->first << std::endl;

            std::vector<Client> clients_id = it->second.clients;

            for (size_t i = 0; i < clients_id.size(); i++) {
                // Accès à l'élément aclients_id clients_id[i]
                std::cout << _clients[clients_id[i].socket.fd].nickname << "|";
            }
            std::cout << std::endl;
        }
    }
}