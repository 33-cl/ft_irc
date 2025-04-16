#include "Command.hpp"

Invite::Invite() {};

Invite::~Invite() {};

void 	Invite::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
		throw recoverable_error(ERR_NOTREGISTERED("*"));
	if (args.size() < 2)
		throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "INVITE"));

	std::string target, channelName;

	target = args[1];
	channelName = args[2];

	if (server._channels.find(channelName) == server._channels.end())
			throw recoverable_error(ERR_NOSUCHCHANNEL(client.nickname, channelName));
	if (client.nickname == target)
			throw recoverable_error(ERR_CANNOTINVITESELF(client.nickname));

	Channel& channel = server._channels[channelName];
	if (!channel.hasClient(client.socket.fd))
		throw recoverable_error(ERR_NOTONCHANNEL(client.nickname, channelName));

	// si on accepte &en masque de channel faut le mettre partout
	if (channelName[0] != '#' && channelName[0] != '&')
		throw recoverable_error(ERR_BADCHANMASK(channelName));

	channel.addInvite(server.find_client(target));
	server.find_client(target).write(":" + client.get_mask() + " INVITE " + target + " " + channelName);
	client.write(":" + server._name + " " + RPL_INVITING(client.nickname, channelName, target));
}


