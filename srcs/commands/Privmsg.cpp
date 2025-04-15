#include "Command.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}


void 	Privmsg::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED("*"));

	if (args.size() < 3)
		throw	recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "PRIVMSG"));
	else if (args.size() > 3)
		throw	recoverable_error(ERR_TOOMANYPARAMS(client.nickname, "PRIVMSG"));

	// Privmsg to channel
	if (!args[1].empty() && args[1][0] == '#')
	{
		// Check channel exists
		std::map<std::string, Channel>::iterator it = server._channels.find(args[1]);
        if (it == server._channels.end())
            throw recoverable_error(ERR_NOSUCHCHANNEL(client.nickname, args[1]));

		// Check client is in the channel
		Channel& channel = it->second;
        if (!channel.hasClient(client.socket.fd))
            throw recoverable_error(ERR_NOTONCHANNEL(client.nickname, args[1]));

		if (!args[2].empty() && args[2][0] == ':')
		{
			std::string to_send = client.get_mask() + "PRIVMSG " + args[1] + " " + args[2];
			server._channels[args[1]].broadcast(to_send, client);
		}
	}
	// Privmsg to client
	else
	{
		if (!Nick::is_nickname_valid(args[1]))
			throw recoverable_error(ERR_ERRONEUSNICKNAME(args[1]));
		if (!args[2].empty() && args[2][0] == ':')
		{
			std::string to_send = client.get_mask() + "PRIVMSG " + args[1] + " " + args[2];
			Client	dest = server.find_client(args[1]);
			dest.write(to_send);
		}
	}
}