#include "Server.hpp"

void    Server::new_client(std::vector<pollfd> &fds)
{
    sockaddr_in client_addr;
    socklen_t   client_len = sizeof(client_addr);
    int         client_fd;

    client_fd = accept(_fd, (sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
        return;

    if (!set_non_blocking(client_fd))
    {
        std::cerr << "Failed to set client socket to non-blocking mode" << std::endl;
        close(client_fd);
        return;
    }

    std::cout << "Client connecte\n";

    Client  new_client(client_fd);

    _clients[client_fd] = new_client;
    fds.push_back(new_client.socket);
}

/*
    PASS aaaa\r\nNICK maeferre\r\nUSER mael 0 * :real name\r\n

*/

std::vector<std::string> split_inputs(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = 0;
    
    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        // Inclure le délimiteur à la fin de chaque token
        tokens.push_back(str.substr(start, end - start + delimiter.length()));
        start = end + delimiter.length();
    }
    
    // Ajouter le reste de la chaîne si elle ne se termine pas par le délimiteur
    if (start < str.length())
        tokens.push_back(str.substr(start));
    
    return tokens;
}

void Server::process_client_data(std::vector<pollfd> &fds, int client_index)
{
    char buffer[1024];
    ssize_t bytes_received = recv(fds[client_index].fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        
        // Récupérer le client actuel
        Client& current_client = _clients[fds[client_index].fd];
        
        // Ajouter les nouvelles données au buffer existant
        std::string current_data = current_client.get_buffer() + std::string(buffer);
        
        // Vérifier si l'entrée se termine par "\n"
        bool is_complete = (current_data.size() > 0 && current_data[current_data.size() - 1] == '\n');
        
        if (is_complete)
        {
            // Traiter l'entrée complète
            if (current_data.size() >= 4) // Garder votre vérification de taille minimale
            {
                std::vector<std::string> commands = split_inputs(current_data, "\r\n");
                
                try
                {
                    for (size_t i = 0; i < commands.size(); i++)
                    {
                        if (commands[i].size() >= 2 && commands[i].substr(commands[i].size() - 2) == "\r\n")
                        {
                            std::string cmd = commands[i].substr(0, commands[i].size() - 2);
                            std::cout << "command: " << cmd << std::endl;
                            process_input(cmd, current_client);
                        }
                    }
                }
                catch(const recoverable_error& e)
                {
                    current_client.send_msg(std::string(e.what()));
                }
            }
            
            // Effacer le buffer après traitement
            current_client.clear_buffer();
        }
        else
        {
            // Stocker l'entrée incomplète dans le buffer du client
            current_client.clear_buffer();
            current_client.append_to_buffer(current_data);
            std::cout << "Storing incomplete data in buffer. Current buffer: " << current_client.get_buffer() << std::endl;
        }
    }
    else
    {
        std::string message = _clients[fds[client_index].fd].get_mask() + "QUIT :";
        remove_client(_clients[fds[client_index].fd], message);
        throw quit_server("");
    }
}


void Server::process_input(std::string& input, Client &client)
{
    std::vector<std::string> args = split_white_spaces(input);

    if (_commands.find(args[0]) == _commands.end())
        return;

    _commands[args[0]]->execute(client, args, *this);
}

