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

	// Utiliser la nouvelle fonction qui combine la suppression et la diffusion des listes
	server.remove_and_broadcast_list(client);
	
	// Garder le client_remove qui gère la deconnexion du client
	server.remove_client(client, message);

	throw quit_server("QUIT command called");
}
