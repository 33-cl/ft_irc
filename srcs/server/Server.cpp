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

void    Server::process_client_data(std::vector<pollfd> &fds, int i)
{
    // Message d'un client existant
    char    buffer[1024];
    ssize_t bytes_received = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0)
    {
        // Afficher le message recu
        buffer[bytes_received] = '\0';
        try
        {
            process_input(std::string(buffer), _clients[fds[i].fd]);
        }
        catch(const std::exception& e)
        {
            //std::cerr << e.what() << '\n';
            if (send(fds[i].fd, e.what(), strlen(e.what()), 0) == -1) // Afficher le message d'erreur au client
                throw std::runtime_error("send() failed");
        }
    }
    else
    {
        // Supprimer le client deconnecte
        std::cout << "Client deconnecte\n";
        close(fds[i].fd);
        fds.erase(fds.begin() + i);
        i--;
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

    // Vérifier si le client est enregistré
    if (client.get_status() != REGISTERED)
    {
        std::string error_msg = "ERROR :You must be registered to create a channel\r\n";
        send(client.get_socket().fd, error_msg.c_str(), error_msg.length(), 0);
        return;
    }
    
    if (_channels.find(channel_name) == _channels.end())
    {
        _channels.insert(std::pair<std::string, Channel>(channel_name, Channel(channel_name, client.get_socket().fd)));
        std::cout << "Channel " << channel_name << " created by " << client.get_nickname() << std::endl;
    }
}

void    Server::process_input(const std::string& input, Client &client)
{
    if (client.get_status() != REGISTERED)
        client.login(input, _password);
    else
    {
        if (input.size() > 6 && !input.compare(0, 5, "JOIN "))
        {
            std::string channel_name = input.substr(5, input.length() - 5 - 1);
            if (_channels.find(channel_name) == _channels.end())
                create_channel(channel_name, client);
            else
                client.join(channel_name, _channels);
        }
        if (input.size() > 6 && !input.compare(0, 8, "PRIVMSG "))
        {
            std::string dest = input.substr(5, input.length() - 5 - 1);
            
        }
    }
}


void    Server::infos()
{
    std::cout << "------------------------\n------------------------\n";

    std::cout << "port     : " << _port     << std::endl
              << "password : " << _password << std::endl
              << "server fd: " << _fd       << std::endl;
    
    // Afficher les informations des clients
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
    
    // Afficher les informations des channels
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