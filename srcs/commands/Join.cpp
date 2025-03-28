#include "Command.hpp"

Join::Join() {}

Join::~Join() {}

/*
    Problemes 
        - "JOIN #general" puis "JOIN #general #autre" cree 2 channels #general

    Erreurs non gerees
        - nom de channel invalide
        - 2eme argument non execute si premier invalide
*/

void    Join::execute(Client& client, std::vector<std::string>& args, Server& server)
{
    if (client.status != REGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED("*"));

    for (size_t i = 1; i < args.size(); i++)
    {
        std::string channel_name = args[i];

        if (!is_channel_valid(channel_name))
            throw recoverable_error(ERR_BADCHANMASK(channel_name));


        // Si le channel n'existe pas on le cree puis on ajoute le client
        if (server._channels.find(channel_name) == server._channels.end())
        {
            server._channels.insert(std::pair<std::string, Channel>(channel_name, Channel(channel_name, client)));
            std::cout << "Channel " << channel_name << " created by " << client.nickname << std::endl;
        }
        // Sinon on ajoute le client direct
        else
        {
            // Vérifier si le client est déjà dans le channel
            std::vector<Client> channel_clients = server._channels[channel_name].clients;
            for (std::vector<Client>::iterator it = channel_clients.begin(); it != channel_clients.end(); ++it)
            {
                if (it->socket.fd == client.socket.fd)
                    throw recoverable_error(ERR_USERONCHANNEL(client.nickname, channel_name));
            }

            server._channels[channel_name].add_client(client.socket.fd);
            std::string confirm_msg = ":" + client.nickname + " JOIN " + channel_name + "\r\n";
            send(client.socket.fd, confirm_msg.c_str(), confirm_msg.length(), 0);
        }

        // Envoyer la reponse JOIN au client 
        std::string join_msg = client.get_mask() + "JOIN " + channel_name + "\r\n";
        send(client.socket.fd, join_msg.c_str(), join_msg.length(), 0);

        Channel& channel = server._channels[channel_name];
        std::string user_list;
        const std::vector<Client>& clients = channel.get_clients();
        
        for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
            if (!user_list.empty())
                user_list += " ";
            user_list += it->nickname;
        }

        client.write(":server 353 " + client.nickname + " = " + channel_name + " :" + user_list);
        client.write(":server 366 " + client.nickname + " " + channel_name + " :End of /NAMES list");

        server._channels[channel_name].broadcast(":server 353 " + client.nickname + " = " + channel_name + " :" + user_list, client);
        server._channels[channel_name].broadcast(":server 366 " + client.nickname + " " + channel_name + " :End of /NAMES list", client);
    }
}

bool Join::is_channel_valid(std::string channel)
{
    if (channel.empty())
        return false;

    if (channel[0] != '#')
        return false;

    if (channel.length() > 307)
        return false;

    for (size_t i = 1; i < channel.length(); ++i)
    {
        if (channel[i] == ' ' ||
            channel[i] == ',' ||
            channel[i] == '\a' ||
            channel[i] == '\r' ||
            channel[i] == '\n')
        {
            return false;
        }
    }

    return true;
}