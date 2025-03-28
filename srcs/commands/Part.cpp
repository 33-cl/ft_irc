#include "Command.hpp"

Part::Part() {}

Part::~Part() {}

void Part::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
		throw recoverable_error(ERR_NOTREGISTERED("*"));
	(void)args;
	(void)server;

}