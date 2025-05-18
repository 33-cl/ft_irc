#include "Command.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}


void 	Privmsg::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED("*"));

	if (args.size() < 3)
		throw	recoverable_error(ERR_NEEDMOREPARAMS(client.nickname, "PRIVMSG"));
	else if (args.size() > 3)
		throw	recoverable_error(ERR_TOOMANYPARAMS(client.nickname, "PRIVMSG"));

	#ifdef BONUS
	if (handleBot(args, client, server))
        return;
	#endif

	// Privmsg to channel
	if (!args[1].empty() && args[1][0] == '#')
	{
		std::map<std::string, Channel>::iterator it = server._channels.find(args[1]);
        if (it == server._channels.end())
            throw recoverable_error(ERR_NOSUCHCHANNEL(client.nickname, args[1]));

		Channel& channel = it->second;
        if (!channel.hasClient(client.socket.fd))
            throw recoverable_error(ERR_NOTONCHANNEL(client.nickname, args[1]));

		if (!args[2].empty() && args[2][0] == ':')
		{
			std::string to_send = client.get_mask() + "PRIVMSG " + args[1] + " " + args[2];
			server._channels[args[1]].broadcast(to_send, client);
		}
	}
	// Privmsg to client
	else
	{
		if (!Nick::is_nickname_valid(args[1]))
			throw recoverable_error(ERR_ERRONEUSNICKNAME(args[1]));
		if (!args[2].empty() && args[2][0] == ':')
		{
			std::string to_send = client.get_mask() + "PRIVMSG " + args[1] + " " + args[2];
			Client&	dest = server.find_client(args[1]);
			dest.write(to_send);
		}
	}
}

#ifdef BONUS

namespace
{
	enum Choice { ROCK = 0, PAPER = 1, SCISSORS = 2, INVALID = -1 };

	int map_choice(const std::string& s)
	{
		if (s == "rock")
			return ROCK;
		if (s == "paper")
			return PAPER;
		if (s == "scissors")
			return SCISSORS;
		return INVALID;
	}
}

bool Privmsg::handleBot(const std::vector<std::string>& args, Client& client, Server& server)
{
    const std::string&	target  = args[1];
    std::string			message = args[2].size() > 0 ? args[2].substr(1) : "";

    if (target != "Bot")
        return false;
    std::string dest = client.nickname;

    //!time
    if (message == "time" || message == "!time")
    {
        time_t now = std::time(NULL);
        char tbuf[9];
        std::strftime(tbuf, sizeof(tbuf), "%H:%M:%S", std::localtime(&now));

        std::string reply = ":Bot!" + server._name + " PRIVMSG " + dest + " :It's " + tbuf;
        client.write(reply);
        return true;
    }

    //!rps rock|paper|scissors
    const std::string cmd = "!rps ";
    if (message.size() > cmd.size() && message.compare(0, cmd.size(), cmd) == 0)
    {
        std::string choice = message.substr(cmd.size());
        int user = map_choice(choice);
        if (user == INVALID)
        {
            client.write(":Bot!" + server._name + " PRIVMSG " + dest + " :Usage: !rps rock|paper|scissors");
            return true;
        }

        int bot = std::rand() % 3;
        const char* names[3] = { "rock", "paper", "scissors" };
        int result = (3 + user - bot) % 3;
        std::string outcome;
        if
			(result == 0) outcome = "It's a tie!";
        else if (result == 1)
			outcome = "You win!";
        else
			outcome = "I win!";

        std::string reply = ":Bot!" + server._name + " PRIVMSG " + dest + " :You played " + names[user] + ", I played " + names[bot] + " — " + outcome;
        client.write(reply);
        return true;
    }

    //unknown command
    std::string notice = ":Bot!" + server._name + " NOTICE " + dest + " :Unknown command. Use !time or !rps.";
    
	client.write(notice);
    return true;
}
#endif