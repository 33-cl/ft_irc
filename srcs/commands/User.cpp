#include "Command.hpp"

User::User() {}

User::~User() {}

void User::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	(void)server;
    if (client.status != PENDING_REGISTRATION)
        return;
    
    std::string new_username, new_realname;
    std::string format = "0 *";
    
    if (args.size() < 4)
        throw std::invalid_argument(ERR_NEEDMOREPARAMS(client.nickname.empty() ? "*" : client.nickname, "USER"));
    
    new_username = args[1];
    
    if (args.size() >= 4) {
        for (size_t i = 3; i < args.size(); ++i) {
            if (i == 3 && args[i][0] == ':') {
                new_realname += args[i].substr(1);
            } else if (i == 3) {
                new_realname += args[i];
            } else {
                new_realname += " " + args[i];
            }
        }
    }
    
    std::string target = client.nickname.empty() ? "*" : client.nickname;
    
    if (is_valid_username(new_username))
        client.username = new_username;
    else
        throw std::invalid_argument(ERR_NEEDMOREPARAMS(target, "USER"));
    
    if (args[2] != "0" || args[3] != "*")
        throw std::invalid_argument(ERR_NEEDMOREPARAMS(target, "USER"));
    
    while (!new_realname.empty() && (new_realname[new_realname.size()-1] == '\r' || new_realname[new_realname.size()-1] == '\n')) {
        new_realname.erase(new_realname.size()-1);
    }
    
    client.realname = new_realname;
    
    if (client.nickname != "")
        client.status = REGISTERED;
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