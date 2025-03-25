#include "Command.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

/*
	Tout parser
*/

void 	Privmsg::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	// std::cout << "Privmsg COMMAND" << std::endl;
	(void)client;
	(void)args;
	(void)server;

	if (!args[1].empty() && args[1][0] == '#')
	{
		if (!args[2].empty() && args[2][0] == ':')
			server._channels[args[1]].broadcast(args[2]);
	}
	else
		client.write(args[2]); // send to client
}