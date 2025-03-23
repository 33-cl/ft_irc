#include "Command.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

void 	Privmsg::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	// std::cout << "Privmsg COMMAND" << std::endl;
	(void)client;
	(void)args;
	(void)server;

	client.write("Privmsg COMMAND\n");
}