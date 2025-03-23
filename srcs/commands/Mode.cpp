#include "Command.hpp"

Mode::Mode() {}

Mode::~Mode() {}

void 	Mode::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	// std::cout << "Mode COMMAND" << std::endl;
	(void)client;
	(void)args;
	(void)server;

	client.write("Mode COMMAND\n");
}