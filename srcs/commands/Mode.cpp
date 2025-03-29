#include "Command.hpp"

Mode::Mode() {}

Mode::~Mode() {}


//bien checker le nb de params, actu MODE #43 + o admin2 pose soucis

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
		
		//etre op pour pouvoir modifier le mode t
		if (args.size() >= 3 && channel.modes['t'] && !channel.isOperator(client))
			throw recoverable_error(ERR_NOTCHANNELOP(client.nickname, target));
			
		if (args.size() < 3)
		{
			client.write(":" + server._name + " " + RPL_CHANNELMODEIS(client.nickname, target, channel.getModesString()));
			return;
		}
		std::string modeChanges = args[2];
		
		//add password or nbusers limit
		std::vector<std::string> modeParams;
        for (size_t i = 3; i < args.size(); ++i)
            modeParams.push_back(args[i]);
	
		
		if (!channel.changeMode(modeChanges, modeParams))
		{
			client.write(":" + server._name + " " + ERR_UNKNOWNMODE(client.nickname, target));
			return;
		}
	
		//message construction with modes str if l or k
		std::string modeBroadcast = ":" + client.get_mask() + " MODE " + target + " " + modeChanges;
		for (std::vector<std::string>::const_iterator it = modeParams.begin(); it != modeParams.end(); ++it)
			modeBroadcast += " " + *it;
		channel.broadcast(modeBroadcast, client);
	} 
	else
		client.write(":" + server._name + " 501 " + client.nickname + " :User mode changes not implemented");
}