#include "Command.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void 	Kick::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	//AJOUTER VERIF OPERATOR
	//Faire qu'on ne puisse pas se kick soit meme
	if (client.status != REGISTERED)
		throw recoverable_error(ERR_NOTREGISTERED("*"));

	if (args.size() < 3)
		throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "KICK"));
	std::string channelName = args[1];
	std::string targetNickname = args[2];
	std::string comment;
	if (args.size() > 3)
	{
		if (!args[3].empty() && args[3][0] == ':')
			comment = args[3].substr(1);
		else
			comment = args[3];
		// add the rest of the comment
		for (size_t i = 4; i < args.size(); ++i)
			comment += " " + args[i];
	}

	if (server._channels.find(channelName) == server._channels.end())
		throw recoverable_error(ERR_NOSUCHCHANNEL(client.nickname, channelName));
	
	//is the operator is present on the chan
	Channel& channel = server._channels[channelName];
	if (!channel.hasClient(client.socket.fd))
		throw recoverable_error(ERR_NOTONCHANNEL(client.nickname, channelName));

	//nosucknick marche pas bien 
	int targetFd = server.getFdByNickname(targetNickname);
	if (targetFd == -1)
		throw recoverable_error(ERR_NOSUCHNICK(client.nickname, targetNickname));
	if (!channel.hasClient(targetFd))
		throw recoverable_error(ERR_USERNOTINCHANNEL(client.nickname, targetNickname, channelName));

	
	std::string kickMessage = ":" + client.get_mask() + " KICK " + channelName + " " + targetNickname + " :" + comment;

	channel.broadcast(kickMessage, client);
    channel.removeClient(targetFd);
}