#include "Command.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void 	Quit::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	// std::cout << "Quit COMMAND" << std::endl;
	(void)client;
	(void)args;
	(void)server;

	client.write("Quit COMMAND\n");
}