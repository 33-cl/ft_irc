#include "Server.hpp"

/*
    Some sort of ft_split() for strings
*/

std::vector<std::string> split(const std::string& str, const std::string& delimiter) 
{
    std::vector<std::string> tokens;
    size_t start = 0;              
    size_t end = str.find(delimiter);

    // Dividing the string
    while (end != std::string::npos) 
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    // Add the last string 
    if (start < str.length()) {
        tokens.push_back(str.substr(start));
    }

    return tokens;
}

/*
    Prints all the informations of the server

    Tool for debugging
*/

void    Server::infos()
{
    std::cout << "------------------------\n------------------------\n";

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
                      << "    Nickname: " << it->second.get_nickname() << std::endl
                      << "    Username: " << it->second.get_username() << std::endl
                      << "    Status  : " << it->second.get_status() << std::endl;
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

            std::vector<int> clients_id = it->second.get_clients();

            for (size_t i = 0; i < clients_id.size(); i++) {
                // Accès à l'élément aclients_id clients_id[i]
                std::cout << _clients[clients_id[i]].get_nickname() << "|";
            }
            std::cout << std::endl;
        }
    }
}