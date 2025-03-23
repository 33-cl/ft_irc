#include "Command.hpp"

Join::Join() {}

Join::~Join() {}

/*
    Problemes 
        - "JOIN #general" puis "JOIN #general #autre" cree 2 channels #general

    Erreurs non gerees
        - nom de channel invalide 
        - JOIN sans argument          
*/

void    Join::execute(Client& client, std::vector<std::string>& args, Server& server)
{

    for (size_t i = 1; i < args.size(); i++)
    {
        std::string channel_name = args[i];


        // Si le channel n'existe pas on le cree puis on ajoute le client
        if (server._channels.find(channel_name) == server._channels.end())
        {
            server._channels.insert(std::pair<std::string, Channel>(channel_name, Channel(channel_name, client.socket.fd)));
            std::cout << "Channel " << channel_name << " created by " << client.nickname << std::endl;
        }
        // Sinon on ajoute le client direct
        else
        {
            // Vérifier si le client est déjà dans le channel
            std::vector<int> channel_clients = server._channels[channel_name].clients;
            for (std::vector<int>::iterator it = channel_clients.begin(); it != channel_clients.end(); ++it)
            {
                if (*it == client.socket.fd)
                {
                    std::string error_msg = ":server 443 " + client.nickname + " " + channel_name + " :is already on channel\r\n";
                    send(client.socket.fd, error_msg.c_str(), error_msg.length(), 0);
                    return;
                }
            }

            server._channels[channel_name].add_client(client.socket.fd);
            std::string confirm_msg = ":" + client.nickname + " JOIN " + channel_name + "\r\n";
            send(client.socket.fd, confirm_msg.c_str(), confirm_msg.length(), 0);
        }
        
        
        // Envoyer la reponse JOIN au client 
        // std::string client_msg = ":" + client.get_nickname() + "!" + client.get_username() + "@localhost JOIN :" + channel_name + "\r\n";
        // send(client.get_socket().fd, client_msg.c_str(), client_msg.length(), 0);

        // Envoyer la reponse 353 (RPL_NAMREPLY) avec la liste des utilisateurs du canal
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
    
}
