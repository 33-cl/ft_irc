#include "Command.hpp"

Notice::Notice() {}

Notice::~Notice() {}

/*
    NOTICE fonctionne comme PRIVMSG a quelques details pres:
        
        - En cas d'erreur sur le destinataire (format invalide, mauvais channel/client) aucune
            erreur n'est renvoyee.
    
        - Certains bots IRC peuvent avoir des reponses automatiques a certaines commandes (ex: !help)
            quand elles sont envoyees avec un PRIVMSG et non un NOTICE. Ca ne change donc pas
            l'implementation de cette commande.
                -> Quand tu coderas le bot pour le BONUS il faudra donc le prendre en compte
*/


void Notice::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED("*"));

	if (args.size() < 3)
		throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "NOTICE"));
	else if (args.size() > 3)
		throw recoverable_error(ERR_TOOMANYPARAMS(client.nickname, "NOTICE"));

	// NOTICE to channel
	if (!args[1].empty() && args[1][0] == '#')
	{
		std::map<std::string, Channel>::iterator it = server._channels.find(args[1]);
        if (it == server._channels.end())
            return;

		Channel& channel = it->second;

		if (!channel.hasClient(client.socket.fd))
            return;

		if (!args[2].empty() && args[2][0] == ':')
		{
			std::string to_send = client.get_mask() + " NOTICE " + args[1] + " " + args[2];
			channel.broadcast(to_send, client);
		}
	}
	// NOTICE to client
	else
	{
		if (!Nick::is_nickname_valid(args[1]))
			return;

		if (!args[2].empty() && args[2][0] == ':')
		{
			std::string to_send = client.get_mask() + " NOTICE " + args[1] + " " + args[2];
			try {
				Client dest = server.find_client(args[1]);
				dest.write(to_send);
			}
			catch (const std::exception& e) {
				// Do nothing, NOTICE doesn't generate an error response
			}
		}
	}
}