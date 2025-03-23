#include "Command.hpp"

Nick::Nick() {}

Nick::~Nick() {}

void 	Nick::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status == UNREGISTERED)
		return; // a gerer
	
	if (args.size() == 0)
		throw std::invalid_argument(ERR_NONICKNAMEGIVEN(std::string("*")));
	if (args.size() != 2)
		return; // too many params

	std::string new_nickname = args[1];

	for (size_t i = 0; i < new_nickname.length(); ++i)
	{
		if ((i == 0 && !isalpha(new_nickname[i])) || !is_nickname_char(new_nickname[i]) || i > 8)
			throw std::invalid_argument(ERR_ERRONEUSNICKNAME(new_nickname));
	}

	if (nick_already_used(new_nickname, server._clients, client))
		throw std::invalid_argument(ERR_NICKNAMEINUSE(new_nickname));

	client.nickname = new_nickname;
	if (client.username != "")
		client.status = REGISTERED;
}

// UTILS

bool Nick::is_nickname_char(char c)
{
	return std::isalnum(c) ||
			c == '-' || c == '_' ||
			c == '[' || c == ']' ||
			c == '{' || c == '}' ||
			c == '\\' || c == '|' || c == '^';
}

bool Nick::nick_already_used(const std::string& nick, const std::map<int, Client>& clients, const Client& current_client)
{
	for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		// Ne pas comparer au client actu
		if (it->second.nickname == nick && &it->second != &current_client)
			return true;
	}
	return false;
}