#include "Command.hpp"

Part::Part() {}

Part::~Part() {}

// void Part::execute(Client& client, std::vector<std::string>& args, Server& server)
// {
// 	if (client.status != REGISTERED)
// 		throw recoverable_error(ERR_NOTREGISTERED("*"));

// 	if (args.size() < 2)
// 		throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "PART"));

// 	std::string channelName = args[1];
// 	std::string reason;

// 	if (args.size() > 2)
// 	{
// 		if (args[2].empty() || args[2][0] != ':')
// 			throw recoverable_error(ERR_INVALIDPARTMESSAGE(client.nickname));
// 		reason = args[2].substr(1);
// 	}

// 	if (server._channels.find(channelName) == server._channels.end())
// 		throw recoverable_error(ERR_NOSUCHCHANNEL(client.nickname, channelName));

// 	Channel& channel = server._channels[channelName];
// 	if (!channel.hasClient(client.socket.fd))
// 		throw recoverable_error(ERR_NOTONCHANNEL(client.nickname, channelName));

// 	std::string partMessage = ":" + client.get_mask() + " PART " + channelName;
// 	if (!reason.empty())
// 		partMessage += " :" + reason;

// 	channel.broadcast(partMessage, client);
// 	channel.removeClient(client.socket.fd);
// 	channel.removeInvite(client);
// 	channel.removeOperator(client);

// }

void Part::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
		throw recoverable_error(ERR_NOTREGISTERED("*"));

	if (args.size() < 2)
		throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "PART"));

	std::vector<std::string> channelNames = split_channels_for_part(args[1]);
	if (channelNames.empty())
		throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "PART"));

	std::string reason;
	if (args.size() > 2)
	{
		if (args[2].empty() || args[2][0] != ':')
			throw recoverable_error(ERR_INVALIDPARTMESSAGE(client.nickname));
		reason = args[2].substr(1);
	}

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string channelName = channelNames[i];
		if (server._channels.find(channelName) == server._channels.end())
		{
			client.write(ERR_NOSUCHCHANNEL(client.nickname, channelName));
			continue;
		}

		Channel& channel = server._channels[channelName];
		if (!channel.hasClient(client.socket.fd))
		{
			client.write(ERR_NOTONCHANNEL(client.nickname, channelName));
			continue;
		}
		std::string partMessage = ":" + client.get_mask() + " PART " + channelName;
		if (!reason.empty())
			partMessage += " :" + reason;
			
		channel.broadcast(partMessage, client);
		channel.removeClient(client.socket.fd);
		channel.removeInvite(client);
		channel.removeOperator(client);
	}
}
