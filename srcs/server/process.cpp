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

void    Server::process_client_data(std::vector<pollfd> &fds, int client_index)
{
    char    buffer[1024];
    ssize_t bytes_received = recv(fds[client_index].fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
    
        std::string buffer_str(buffer);
        
        if (buffer_str.size() < 4)
            return;

        std::vector<std::string> commands = split_inputs(buffer_str, "\n");

        try
        {
            for (size_t i = 0; i < commands.size(); i++)
            {
                std::string cmd = commands[i];
                
                if (cmd.size() >= 2 && cmd.substr(cmd.size() - 2) == "\r\n")
                {
                    cmd = cmd.substr(0, cmd.size() - 2);
                    process_input(cmd, _clients[fds[client_index].fd]);
                }
            }
        }
        catch(const recoverable_error& e)
        {
            _clients[fds[client_index].fd].send_msg(std::string(e.what()));
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

