#include "Command.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void   Quit::execute(Client& client, std::vector<std::string>& args, Server& server)
{
    if (client.status != REGISTERED)
    {
        throw recoverable_error(ERR_NOTREGISTERED("*"));
    }
	
	std::string quitMessage;
	if (args.size() <= 1 || args[1].empty())
		quitMessage = "Client disconnected";
	else
	{
		quitMessage = args[1];
		for (size_t i = 2; i < args.size(); ++i)
			quitMessage += " " + args[i];
	}
	std::string message = client.get_mask() + " QUIT :" + quitMessage;
    std::cout << "|" << message << "|\n";

    for (std::map<std::string, Channel>::iterator it = server._channels.begin();
         it != server._channels.end(); ++it)
    {
        Channel& channel = it->second;
        if (channel.hasClient(client.socket.fd))
        {
            channel.broadcast(message, client);
            channel.removeClient(client.socket.fd);
			close(client.socket.fd);
        }
    }
}

