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
    fds.push_back(new_client.get_socket());
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
                std::cout << commands[i];
                process_input(commands[i], _clients[fds[client_index].fd]);
            }
            std::cout << std::endl;
        }
        catch(const std::exception& e)
        {
            //std::cerr << e.what() << '\n';
            send(fds[client_index].fd, e.what(), strlen(e.what()), 0);
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

void    Server::create_channel(const std::string& channel_name, Client& client)
{
    // Vérifier si le nom du canal est valide
    if (channel_name.empty())
    {
        std::string error_msg = "ERROR :Invalid channel name\r\n";
        send(client.get_socket().fd, error_msg.c_str(), error_msg.length(), 0);
        return;
    }

    if (_channels.find(channel_name) == _channels.end())
    {
        _channels.insert(std::pair<std::string, Channel>(channel_name, Channel(channel_name, client.get_socket().fd)));
        std::cout << "Channel " << channel_name << " created by " << client.get_nickname() << std::endl;
    }
}


void Server::process_input(std::string& input, Client &client)
{
    std::vector<std::string> args = split(input, " ");

    if (client.get_status() != REGISTERED)
        client.login(input, _password);
    else
    {
        if (check_command(input, "JOIN"))
        {
            _commands["JOIN"]->execute(client, args);

            // std::string channel_name = input.substr(5, input.length() - 5);
            // if (_channels.find(channel_name) == _channels.end())
            //     create_channel(channel_name, client);
            // else
            //     client.join(channel_name, _channels);

            // // Envoie la réponse JOIN au client
            // std::string client_msg = ":" + client.get_nickname() + "!" + client.get_username() + "@localhost JOIN :" + channel_name + "\r\n";
            // send(client.get_socket().fd, client_msg.c_str(), client_msg.length(), 0);

            // // Envoie la réponse 353 (RPL_NAMREPLY) avec la liste des utilisateurs du canal
            // std::string names_msg = ":ircserv 353 " + client.get_nickname() + " = " + channel_name + " :";
            // Channel& channel = _channels[channel_name];
            // std::vector<int> client_fds = channel.get_clients();

            // for (size_t i = 0; i < client_fds.size(); ++i)
            // {
            //     int fd = client_fds[i];
            //     if (_clients.find(fd) != _clients.end())
            //     {
            //         names_msg += _clients[fd].get_nickname();
            //         if (i < client_fds.size() - 1)
            //             names_msg += " ";
            //     }
            // }
            // names_msg += "\r\n";
            // send(client.get_socket().fd, names_msg.c_str(), names_msg.length(), 0);
        }
        else if (check_command(input, "PRIVMSG"))   {(void)input;}
        else if (check_command(input, "NICK"))      {(void)input;}
        else if (check_command(input, "TOPIC"))     {(void)input;}
        else if (check_command(input, "MODE"))      {(void)input;}
        else if (check_command(input, "TOPIC"))     {(void)input;}
    }
}