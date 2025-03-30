#include "Command.hpp"

Join::Join() {}

Join::~Join() {}

void    Join::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	
    if (client.status != REGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED("*"));

    for (size_t i = 1; i < args.size(); i++)
    {
        std::string		channel_name = args[i];

		
        if (!is_channel_valid(channel_name))
			throw recoverable_error(ERR_BADCHANMASK(channel_name));
		

        // Si le channel n'existe pas on le cree puis on ajoute le client
        if (server._channels.find(channel_name) == server._channels.end())
        {	
			// isChannel = false;
            server._channels.insert(std::pair<std::string, Channel>(channel_name, Channel(channel_name, client)));
            std::cout << "Channel " << channel_name << " created by " << client.nickname << std::endl;
        }
        // Sinon on ajoute le client direct
        else
        {
			Channel&		channel = server._channels[channel_name];

            if (is_in_channel(client, channel))
            {
                client.write(ERR_USERONCHANNEL(client.nickname, channel_name));
                continue;
            }

			//invite only?is invited?
			if (channel.modes['i'] && !channel.isInvited(client))
            {
                client.write(ERR_NOTINVITED(client.nickname, channel_name));
                continue;
            }

			if (channel.modes['k'])
			{
				//on saute le mdp si on est en mode k ppour ne pasa faire de check dessus 
				if (i + 1 >= args.size() || args[i + 1] != channel.password)
                {
                    client.write(ERR_BADCHANNELKEY(client.nickname, channel_name));
                    continue;
                }
				i++;
			}
			//ici j'envoi client plutot que le fd
            server._channels[channel_name].add_client(client);
            std::string confirm_msg = ":" + client.nickname + " JOIN " + channel_name + "\r\n";
            send(client.socket.fd, confirm_msg.c_str(), confirm_msg.length(), 0);

        }

        // Envoyer la reponse JOIN au client 
        std::string join_msg = client.get_mask() + "JOIN " + channel_name;
        // send(client.socket.fd, join_msg.c_str(), join_msg.length(), 0);
        client.write(join_msg);

        server._channels[channel_name].broadcast(join_msg, client);

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

        // server._channels[channel_name].broadcast(":server 353 " + client.nickname + " = " + channel_name + " :" + user_list, client);
        // server._channels[channel_name].broadcast(":server 366 " + client.nickname + " " + channel_name + " :End of /NAMES list", client);
    }
}

bool Join::is_in_channel(Client& client, Channel& channel)
{
    const std::vector<Client>& clients = channel.get_clients();
    for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->socket.fd == client.socket.fd)
            return true;
    }
    return false;
}

bool Join::is_channel_valid(const std::string& channel) 
{    
    if (channel.empty() || channel.length() > 50)
        return false;

    if (channel[0] != '#')
        return false;

    if (channel.length() > 1 && isdigit(channel[1]))
        return false;

    for (size_t i = 1; i < channel.length(); ++i) {
        const char c = channel[i];
        if (!isalnum(c) && c != '-' && c != '_' && c != '.') {
            return false;
        }
    }

    if (channel.find(' ') != std::string::npos)
        return false;

    return true;
}