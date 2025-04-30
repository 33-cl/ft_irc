#include "Command.hpp"

Nick::Nick() {}

Nick::~Nick() {}

void Nick::execute(Client& client, std::vector<std::string>& args, Server& server)
{
    if (client.status == UNREGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED(client.nickname));
    
    if (args.size() < 2)
        throw recoverable_error(ERR_NONICKNAMEGIVEN(std::string("*")));

    if (args.size() > 2)
        throw recoverable_error(ERR_TOOMANYPARAMS(client.nickname, "NICK"));

    std::string new_nickname = args[1];
    
    if (!is_nickname_valid(new_nickname))
        throw recoverable_error(ERR_ERRONEUSNICKNAME(new_nickname));

    if (nick_already_used(new_nickname, server._clients, client))
        throw recoverable_error(ERR_NICKNAMEINUSE(new_nickname));

    client.nickname = new_nickname;
    if (client.username != "" && client.status != REGISTERED)
    {
        client.send_msg(RPL_WELCOME(client.nickname, client.get_mask()));
        client.status = REGISTERED;
    }
}

bool Nick::is_nickname_valid(std::string str)
{
    if (str.length() == 0 || str.length() > 9)
        return false;

    if (!std::isalpha(str[0]))
        return false;

    for (size_t i = 1; i < str.length(); ++i)
    {
        if (!(std::isalnum(str[i]) ||
              str[i] == '-' || str[i] == '_' ||
              str[i] == '[' || str[i] == ']' ||
              str[i] == '{' || str[i] == '}' ||
              str[i] == '\\' || str[i] == '|' || str[i] == '^'))
        {
            return false;
        }
    }

    return true;
}

bool Nick::nick_already_used(const std::string& nick, const std::map<int, Client>& clients, const Client& current_client)
{
	for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.nickname == nick && &it->second != &current_client)
			return true;
	}
	return false;
}