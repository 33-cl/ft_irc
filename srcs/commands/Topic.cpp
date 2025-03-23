#include "Command.hpp"

Topic::Topic() {}

Topic::~Topic() {}

void 	Topic::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	// std::cout << "Topic COMMAND" << std::endl;
	(void)client;
	(void)args;
	(void)server;

	client.write("Topic COMMAND\n");
}