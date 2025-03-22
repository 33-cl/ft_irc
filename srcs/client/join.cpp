#include "Client.hpp"

void    Client::join(const std::string& name, std::map<std::string, Channel>& channels)
{
    // Vérifier si le channel existe, sinon il faut traiter ce cas
    if (channels.find(name) == channels.end())
    {
        std::string error_msg = ":server ERROR :Channel " + name + " does not exist\r\n";
        send(socket.fd, error_msg.c_str(), error_msg.length(), 0);
        return;
    }

    // Vérifier si le client est déjà dans le channel
    std::vector<int> channel_clients = channels[name].get_clients();
    for (std::vector<int>::iterator it = channel_clients.begin(); it != channel_clients.end(); ++it)
    {
        if (*it == socket.fd)
        {
            std::string error_msg = ":server 443 " + nickname + " " + name + " :is already on channel\r\n";
            send(socket.fd, error_msg.c_str(), error_msg.length(), 0);
            return;
        }
    }

    // Si le client n'est pas déjà dans le channel, on l'ajoute
    channels[name].add_client(socket.fd);
    std::string confirm_msg = ":" + nickname + " JOIN " + name + "\r\n";
    send(socket.fd, confirm_msg.c_str(), confirm_msg.length(), 0);
}
