#include "Server.hpp"

/*
    Some sort of ft_split() for strings

    Also suppresses "\r\n" at the end of the string
*/

std::vector<std::string> split(const std::string& str, const std::string& delimiter) 
{
    std::vector<std::string> tokens;
    size_t start = 0;              
    size_t end = str.find(delimiter);

    // Dividing the string
    while (end != std::string::npos) 
    {
        std::string token = str.substr(start, end - start);
        tokens.push_back(token);
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    // Add the last string 
    if (start < str.length()) {
        std::string token = str.substr(start);

        // Suppression des "\r\n" à la fin du dernier token en C++98
        while (!token.empty() && (token[token.size()-1] == '\r' || token[token.size()-1] == '\n')) {
            token.erase(token.size()-1);
        }
        
        tokens.push_back(token);
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