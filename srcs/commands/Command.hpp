#pragma once

#include "../messages.hpp"
#include "../client/Client.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"

#include <map>

class Channel;
class Client;
class Server;

class Command
{
    public:
        Command() {};
        virtual ~Command() {};

        virtual void    execute(Client& client, std::vector<std::string>& args, Server &serv) = 0;

        friend class Client;
        friend class Server;
        friend class Channel;
};

class Pass : public Command
{
    public:
        Pass();
        ~Pass();

        void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Nick : public Command
{
    public:
        Nick();
        ~Nick();

        static bool     is_nickname_valid(std::string str);
        bool            nick_already_used(const std::string& nick, const std::map<int, Client>& clients, const Client& current_client);
        void            execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class User : public Command
{
    public:
        User();
        ~User();

        bool	is_valid_username(const std::string& user);
        void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Join : public Command
{
    public:
        Join();
        ~Join();

        std::vector<std::pair<std::string, std::string> >   split_join(const std::string& str);
        bool                                                can_join(Client& client, Channel& channel, const std::string& channel_name, const std::string& password);
        bool                                                is_in_channel(Client& client, Channel& channel);
        static bool                                         is_channel_valid(const std::string& channel);
        void                                                execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Privmsg : public Command
{
    public:
        Privmsg();
        ~Privmsg();

        void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Mode : public Command
{
    public:
        Mode();
        ~Mode();

        void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Kick : public Command
{
	public:
		Kick();
		~Kick();

		void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Topic : public Command
{
    public:
        Topic();
        ~Topic();

        void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Quit : public Command
{
    public:
        Quit();
        ~Quit();

        void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Part	: public Command
{
	public:
		Part();
		~Part();

		void    execute(Client& client, std::vector<std::string>& args, Server &serv);
		std::vector<std::string> split_channels_for_part(const std::string& str);

};

class Invite : public Command
{
	public:
		Invite();
		~Invite();

	void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};