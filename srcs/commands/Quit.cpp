#include "Command.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void   Quit::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	std::string quitMessage;
	if (args.size() <= 1 || args[1].empty())
		quitMessage = "Client disconnected";
	else
	{
		quitMessage = args[1];
		for (size_t i = 2; i < args.size(); ++i)
			quitMessage += " " + args[i];
	}
	std::string message = ":" + client.nickname + "!" + client.username + "@localhost QUIT :" + quitMessage + "\r\n";

    for (std::map<std::string, Channel>::iterator it = server._channels.begin();
         it != server._channels.end(); ++it)
    {
        Channel& channel = it->second;
        if (channel.hasClient(client.socket.fd))
        {
            channel.broadcast(message, client);
            channel.removeClient(client.socket.fd);
        }
    }
}

