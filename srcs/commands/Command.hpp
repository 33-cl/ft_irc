#pragma once

#include "../client/Client.hpp"
#include "../server/Server.hpp"

class Client;
class Server;

class Command
{
    public:
        Command();
        virtual ~Command();

        virtual void    execute(Client& client, std::vector<std::string>& args, Server &server) = 0;

        friend class Client;
        friend class Server;
        friend class Channel;
};

class Join : public Command
{
    public:
        Join();
        ~Join();

        void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Pass : public Command
{
    public:
        Pass();
        ~Pass();

        void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Kick : public Command
{
	public:
		Kick();
		~Kick();

		void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};

class Quit : public Command
{
	public:
		Quit();
		~Quit();

		void    execute(Client& client, std::vector<std::string>& args, Server &serv);
};