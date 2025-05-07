#include "Command.hpp"

Topic::Topic() {}

Topic::~Topic() {}

void 	Topic::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
		throw recoverable_error(ERR_NOTREGISTERED("*"));

	if (args.size() < 2)
	throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "TOPIC"));

	std::string channelName = args[1];

	if (server._channels.find(channelName) == server._channels.end())
		throw recoverable_error(ERR_NOSUCHCHANNEL(client.nickname, channelName));

	std::string topicMsg;
	if (args.size() > 2)
	{
		if (args[2].empty() || args[2][0] != ':')
			throw recoverable_error(ERR_INVALIDKICKMESSAGE(client.nickname));
		topicMsg = args[2].substr(1);
	}
	Channel& channel = server._channels[channelName];
	if (!channel.hasClient(client.socket.fd))
		throw recoverable_error(ERR_NOTONCHANNEL(client.nickname, channelName));
	if (args.size() == 2)
	{
		if (channel.topic.empty())
			client.write(":" + server._name + " " + RPL_NOTOPIC(client.nickname, channelName));
		else
			client.write(":" + server._name + " " + RPL_TOPIC(client.nickname, channelName, channel.topic));
		return;
	}
	if (channel.modes['t'] && !channel.isOperator(client))
		throw recoverable_error(ERR_NOTCHANNELOP(client.nickname, channelName));
	channel.topic = topicMsg;
	std::string topicMessage = client.get_mask() + " TOPIC " + channelName + " :" + topicMsg;
	channel.broadcastEveryone(topicMessage, client);

}