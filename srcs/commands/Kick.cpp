#include "Command.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void 	Kick::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED("*"));

	// std::cout << "KICK COMMAND" << std::endl;
	// (void)client;
	(void)args;
	(void)server;

	client.write("KICK COMMAND\n");
}