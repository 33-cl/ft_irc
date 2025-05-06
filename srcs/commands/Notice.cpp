#include "Command.hpp"

Notice::Notice() {}

Notice::~Notice() {}

/*
   NOTICE is like PRIVMSG but different in some ways:
   
   - If the message goes to wrong person/channel, no error message comes back
   
   - Some bots answer to commands (like !help) only with PRIVMSG, not NOTICE
     But your code for NOTICE stays the same
     -> Remember this when making the bonus bot
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
			channel.broadcastEveryone(to_send, client);
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