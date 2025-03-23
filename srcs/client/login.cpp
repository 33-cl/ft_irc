#include "Client.hpp"

bool	Client::is_valid_username(const std::string& user)
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
bool Client::is_nickname_char(char c)
{
	return std::isalnum(c) ||
			c == '-' || c == '_' ||
			c == '[' || c == ']' ||
			c == '{' || c == '}' ||
			c == '\\' || c == '|' || c == '^';
}

void    Client::login(std::string& input, const std::string& password, Server& server)
{
    if (status == UNREGISTERED)
    {
        if (!check_command(input, "PASS"))
            return;
        std::cout << "password received : |" << input.substr(5, input.length() - 5) << "|" << std::endl;
        std::cout << "real password     : |" << password << "|" << std::endl;
        if (input.substr(5, input.length() - 5) != password) // substr() exclut le \n a la fin du buffer
            throw std::invalid_argument(ERR_PASSWDMISMATCH(std::string("*")));
        status = PENDING_REGISTRATION;
    }
    else if (status == PENDING_REGISTRATION)
    {
        if (input.size() > 5 && !input.compare(0, 5, "NICK "))
        {
            std::string new_nick = input.substr(5, input.length() - 5 - 1);
            
            // std::cout << "|" << new_nick << "|\n";
			if (new_nick.empty())
				throw std::invalid_argument(ERR_NONICKNAMEGIVEN(std::string("*")));

			for (size_t i = 0; i < new_nick.length(); ++i)
			{
				if ((i == 0 && !isalpha(new_nick[i])) || !is_nickname_char(new_nick[i]) || i > 8)
					throw std::invalid_argument(ERR_ERRONEUSNICKNAME(new_nick));
			}
			
			if (nick_already_used(new_nick, server._clients, *this))
				throw std::invalid_argument(ERR_NICKNAMEINUSE(new_nick));

            // Nickname collision KILL
            // if (hasNickCollision(new_nick))
            //     throw std::invalid_argument(":irc.example.com 436 * " + new_nick + " :Nickname collision KILL\r\n");

            // Temporarily unavailable nickname
            // if (isNickReserved(new_nick))
            //     throw std::invalid_argument(":irc.example.com 437 * " + new_nick + " :Nick/channel is temporarily unavailable\r\n");
            
            // Connection restricted
            // if (isConnectionRestricted())
            //     throw std::invalid_argument(":irc.example.com 484 * " + new_nick + " :Your connection is restricted\r\n");
            
            nickname = new_nick;
            if (username != "")
                status = REGISTERED;
        }
        if (input.size() > 6 && !input.compare(0, 5, "USER "))
        {
			std::istringstream iss(input);
			std::string cmd, format, username, realname;
				
			iss >> cmd >> username;
			std::getline(iss, format, ':');
			std::getline(iss, realname);

			size_t first = format.find_first_not_of(" \t");
			size_t last = format.find_last_not_of(" \t");
			if (first != std::string::npos)
				format = format.substr(first, last - first + 1);
			else
				format = "";

			std::string target = nickname.empty() ? "*" : nickname;

			if (is_valid_username(username))
				this->username = username;
			else
				throw std::invalid_argument(ERR_NEEDMOREPARAMS(target, "USER"));
			
			if (format != "0 *")
				throw std::invalid_argument(ERR_NEEDMOREPARAMS(target, "USER"));
			
			if (realname.find('\r') != std::string::npos || realname.find('\n') != std::string::npos)
  				throw std::invalid_argument(ERR_NEEDMOREPARAMS(target, "USER"));
			else
				this->realname = realname;
            
			if (nickname != "")
                status = REGISTERED;
        }
    }
}

bool Client::nick_already_used(const std::string& nick, const std::map<int, Client>& clients, const Client& current_client)
{
	for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		// Ne pas comparer au client actu
		if (it->second.get_nickname() == nick && &it->second != &current_client)
			return true;
	}
	return false;
}

