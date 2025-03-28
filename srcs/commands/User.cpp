#include "Command.hpp"

User::User() {}

User::~User() {}

void User::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	(void)server;
    if (client.status == UNREGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED(client.nickname));
    else if (client.status == REGISTERED)
        throw recoverable_error(ERR_ALREADYREGISTERED(client.nickname));
    
    std::string new_username, new_realname;

    if (args.size() < 5)
        throw recoverable_error(ERR_NEEDMOREPARAMS(client.nickname.empty() ? "*" : client.nickname, "USER"));
    
    new_username = args[1];
	if (args[4].empty() || args[4][0] != ':')
		throw recoverable_error(ERR_INVALIDUSERFORMAT(client.nickname));
	new_realname = args[4].substr(1);

    std::string target = client.nickname.empty() ? "*" : client.nickname;

    if (!is_valid_username(new_username))
        throw recoverable_error(ERR_NEEDMOREPARAMS(target, "USER"));
    
    if (args[2] != "0" || args[3] != "*")
    	throw recoverable_error(ERR_NEEDMOREPARAMS(target, "USER"));
    
    while (!new_realname.empty() && (new_realname[new_realname.size()-1] == '\r' || new_realname[new_realname.size()-1] == '\n'))
	{
        new_realname.erase(new_realname.size()-1);
    }
    
    client.username = new_username;
    client.realname = new_realname;

    if (client.nickname != "")
    {
        client.send_msg(RPL_WELCOME(client.nickname, client.get_mask()));
        client.status = REGISTERED;
    }
}

bool	User::is_valid_username(const std::string& user)
{
	if (user.empty() || user.length() > 9)
		return false;
	for (size_t i = 0; i < user.length(); ++i)
	{
		if (isspace(user[i]) || user[i] == '\r' || user[i] == '\n')
			return false;
	}
	return true;
}