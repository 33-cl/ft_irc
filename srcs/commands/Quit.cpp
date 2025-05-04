#include "Command.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void   Quit::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	std::string quitMessage;
	if (args.size() > 1 && (!args[1].empty() && args[1][0] != ':'))
		throw recoverable_error(ERR_INVALIDQUITMESSAGE(client.nickname));
	if (args.size() <= 1 || args[1].size() <= 1)
		quitMessage = "Client disconnected";
	else
		quitMessage = args[1].substr(1);

	std::string message = client.get_mask() + "QUIT :" + quitMessage;


    std::vector<std::string> channelsToDestroy;
    for (std::map<std::string, Channel>::iterator it = server._channels.begin();
         it != server._channels.end(); ++it) 
    {
        Channel& channel = it->second;
        if (channel.hasClient(client.socket.fd)) 
        {
            channel.removeClient(client.socket.fd);
            channel.removeOperator(client);
            
            if (!channel.has_operator())
                channelsToDestroy.push_back(channel.name);
        }
    }

    for (std::vector<std::string>::iterator it = channelsToDestroy.begin();
         it != channelsToDestroy.end(); ++it)
    {
        if (server._channels.find(*it) != server._channels.end())
            server.destroy_channel(server._channels[*it]);
    }

	server.remove_client(client, message);
	server.broadcast_channel_lists();

	throw quit_server("QUIT command called");
}
