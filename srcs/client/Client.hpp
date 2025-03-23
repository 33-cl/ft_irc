#pragma once

#include "../channel/Channel.hpp"
#include "../server/Server.hpp"
#include "../messages.hpp"

#include <poll.h>
#include <arpa/inet.h>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstdio>

enum status {
    UNREGISTERED,
    PENDING_REGISTRATION,
    REGISTERED,
};

class Server;

class Client
{
    private:
        pollfd      socket;
        std::string nickname;
        std::string username;
		std::string realname;
        std::string hostname;
        short       status;

    public:
        Client();
        Client(int fd);
        ~Client();

        void    write(const std::string& str);

        void    login(std::string& input, const std::string& password, Server& server);
        void    join(const std::string& name, std::map<std::string, Channel>& channels);

		bool		is_valid_username(const std::string& user);
		bool		is_nickname_char(char c);
		bool		nick_already_used(const std::string& nick, const std::map<int, Client>& clients, const Client& current_client);

        static pollfd  create_socket(int fd, short events, short revents);

        friend class Server;
        friend class Channel;
        friend class Command;
        friend class Pass;
        friend class Nick;
        friend class User;
        friend class Join;
        friend class Privmsg;
        friend class Mode;
        friend class Kick;
        friend class Topic;
        friend class Quit;
};

bool    check_command(std::string& input, const std::string& command);
