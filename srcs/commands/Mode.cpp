#include "Command.hpp"

Mode::Mode() {}

Mode::~Mode() {}


//MODE #3 k yes :irc.example.com 461 admin MODE :Not enough parameters, pb au niveau du message d'erreur?

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

		if (modeChanges.empty())
			throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "MODE"));
		if ((modeChanges[0] != '+' && modeChanges[0] != '-'))
			throw recoverable_error(ERR_INVALIDMODEFLAG(client.nickname));
		if (modeChanges.size() < 2)
			throw recoverable_error(ERR_INVALIDMODESTRING(client.nickname));
		if (modeChanges.find(' ') != std::string::npos)
			throw recoverable_error(ERR_MODE_SPACES(client.nickname));

		//add password or nbusers limit
		std::vector<std::string> modeParams;
        for (size_t i = 3; i < args.size(); ++i)
            modeParams.push_back(args[i]);
	
		
		channel.changeMode(modeChanges, modeParams, client, server);
	
		//message construction with modes str if l or k
		std::string modeBroadcast = ":" + client.get_mask() + " MODE " + target + " " + modeChanges;
		for (std::vector<std::string>::const_iterator it = modeParams.begin(); it != modeParams.end(); ++it)
			modeBroadcast += " " + *it;
		channel.broadcast(modeBroadcast, client);
	} 
	else
		throw recoverable_error(ERR_UMODEUNKNOWNFLAG(client.nickname));
}