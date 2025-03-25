#include "Server.hpp"

void    Server::new_client(std::vector<pollfd> &fds)
{
    sockaddr_in client_addr;
    socklen_t   client_len = sizeof(client_addr);
    int         client_fd;

    client_fd = accept(_fd, (sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
        return;

    std::cout << "Client connecte\n";

    Client  new_client(client_fd);

    _clients[client_fd] = new_client;
    fds.push_back(new_client.socket);
}

void    Server::process_client_data(std::vector<pollfd> &fds, int client_index)
{
    // Message d'un client existant
    char    buffer[1024];
    ssize_t bytes_received = recv(fds[client_index].fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0)
    {
        // Afficher le message recu
        buffer[bytes_received] = '\0';

        std::string buffer_str(buffer);
        std::vector<std::string> commands = split(buffer_str, "\r\n");

        try
        {

            for (size_t i = 0; i < commands.size(); i++)
            {
                //std::cout << "command: " << commands[i] << std::endl;
                process_input(commands[i], _clients[fds[client_index].fd]);
            }
        }
        catch(const recoverable_error& e)
        {
            //std::cerr << e.what() << '\n';
            _clients[fds[client_index].fd].send_msg(std::string(e.what()));
            //send(fds[client_index].fd, e.what(), strlen(e.what()), 0);
        }
    }
    else
    {
        // Supprimer le client deconnecte
        std::cout << "Client deconnecte\n";
        close(fds[client_index].fd);
        fds.erase(fds.begin() + client_index);
        client_index--;
    }
}


void Server::process_input(std::string& input, Client &client)
{
    std::vector<std::string> args = split(input, " ");
    
    // A SUPPRIMER AVANT DE PUSH
    std::cout << "|" << input << "|\n";
    if (input == "ADMIN")
    {
        client.nickname = "admin";
        client.username = "admin";
        client.status = REGISTERED;
    }

    // Cas ou input n'est pas une commande
    if (_commands.find(args[0]) == _commands.end())
        return;

    _commands[args[0]]->execute(client, args, *this);
}