#include "Command.hpp"

Kick::Kick() {}

Kick::~Kick() {}

std::vector<std::string> split_params(const std::string& str)
{
    std::vector<std::string> result;
    std::vector<std::string> tokens = split(str, ","); // Utilisez votre fonction split existante
    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::string trimmed = tokens[i];
        // Suppression des espaces en début et fin de chaîne
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t") + 1);
        if (!trimmed.empty())
            result.push_back(trimmed);
    }
    return result;
}

void Kick::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
		throw recoverable_error(ERR_NOTREGISTERED("*"));

	if (args.size() < 3)
		throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "KICK"));

	std::vector<std::string> channelNames = split_params(args[1]);
	std::vector<std::string> targetNicknames = split_params(args[2]);

	if (channelNames.size() != targetNicknames.size())
   		throw recoverable_error(ERR_MISMATCHMULTIKICK(client.nickname));

	std::string comment;
	if (args.size() > 3)
	{
		if (args[3].empty() || args[3][0] != ':')
			throw recoverable_error(ERR_INVALIDKICKMESSAGE(client.nickname));
		comment = args[3].substr(1);
	}

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string& channelName = channelNames[i];
		std::string& targetNickname = targetNicknames[i];

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
		if (!channel.isOperator(client))
		{
			client.write(ERR_NOTCHANNELOP(client.nickname, channelName));
			continue;
		}
		if (client.nickname == targetNickname)
		{
			client.write(ERR_CANNOTKICKSELF(client.nickname));
			continue;
		}

		int targetFd = server.getFdByNickname(targetNickname);
		if (targetFd == -1)
		{
			client.write(ERR_NOSUCHNICK(client.nickname, targetNickname));
			continue;
		}
		if (!channel.hasClient(targetFd))
		{
			client.write(ERR_USERNOTINCHANNEL(client.nickname, targetNickname, channelName));
			continue;
		}

		std::string kickMessage = ":" + client.get_mask() + " KICK " + channelName + " " + targetNickname;
		if (!comment.empty())
			kickMessage += " :" + comment;

		Client& targetClient = server.find_client(targetNickname);

		channel.broadcast(kickMessage, client);
		channel.removeClient(targetFd);
		channel.removeInvite(targetClient);
		channel.removeOperator(targetClient);

		for (std::vector<Client>::iterator member_it = channel.clients.begin();
			 member_it != channel.clients.end(); ++member_it)
		{
			server.send_user_list(*member_it, channel);
		}

		std::map<int, Client>::iterator kicked_it = server._clients.find(targetFd);
		if (kicked_it != server._clients.end())
			server.send_user_list(kicked_it->second, channel);
	}
}