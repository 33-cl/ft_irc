#include "Command.hpp"

Mode::Mode() {}

Mode::~Mode() {}

/*
   +k <password>:
     - Enables "key" mode
     - A password is required to join the channel
     - Example: MODE #channel +k secretpass
   
   +l <limit>:
     - Sets a user limit for the channel
     - Example: MODE #channel +l 10

   +t:
     - Only operators can change the topic
     - Example: MODE #channel +t

   +i:
     - "Invite-only" mode: only invited users can join
     - Example: MODE #channel +i

   +o <nickname>:
     - Grants operator status to a user
     - Example: MODE #channel +o user1
*/

void 	Mode::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
		throw recoverable_error(ERR_NOTREGISTERED("*"));

	if (args.size() < 2)
		throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "MODE"));

	std::string target = args[1];
	if (target[0] == '#')
	{
		if (server._channels.find(target) == server._channels.end())
			throw recoverable_error(ERR_NOSUCHCHANNEL(client.nickname, target));
		
		Channel& channel = server._channels[target];

		if (!channel.hasClient(client.socket.fd))
			throw recoverable_error(ERR_NOTONCHANNEL(client.nickname, target));
		
		if (args.size() >= 3 && channel.modes['t'] && !channel.isOperator(client))
			throw recoverable_error(ERR_NOTCHANNELOP(client.nickname, target));
			
		if (args.size() < 3)
		{
			client.write(":" + server._name + " " + RPL_CHANNELMODEIS(client.nickname, target, channel.getModesString()));
			return;
		}
		std::string modeChanges = args[2];

		if (modeChanges.empty())
			throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "MODE"));
		if ((modeChanges[0] != '+' && modeChanges[0] != '-'))
			throw recoverable_error(ERR_INVALIDMODEFLAG(client.nickname));
		if (modeChanges.size() < 2)
			throw recoverable_error(ERR_INVALIDMODESTRING(client.nickname));
		if (modeChanges.find(' ') != std::string::npos)
			throw recoverable_error(ERR_MODE_SPACES(client.nickname));

		std::vector<std::string> modeParams;
        for (size_t i = 3; i < args.size(); ++i)
            modeParams.push_back(args[i]);
	
		// bool success = channel.changeMode(modeChanges, modeParams, client, server);
		// if (success)
		// {
		// 	std::string modeBroadcast = ":" + client.get_mask() + " MODE " + target + " " + modeChanges;
		// 	for (std::vector<std::string>::const_iterator it = modeParams.begin(); it != modeParams.end(); ++it)
		// 		modeBroadcast += " " + *it;
		// 	channel.broadcastEveryone(modeBroadcast, client);
		// }
		bool success = channel.changeMode(modeChanges, modeParams, client, server);
		if (success)
		{
			std::string reply = ":" + client.get_mask() + " MODE " + target + " " + channel.getLastAppliedModes();
			const std::vector<std::string>& params = channel.getLastAppliedParams();
			for (size_t i = 0; i < params.size(); ++i)
				reply += " " + params[i];
			channel.broadcastEveryone(reply, client);
		}
	}
	else
		throw recoverable_error(ERR_UMODEUNKNOWNFLAG(client.nickname));
}