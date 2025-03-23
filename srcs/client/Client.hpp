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
	friend class Server;
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

        pollfd      get_socket() const;
        short       get_status() const;
        std::string get_nickname() const;
        std::string get_username() const;
        std::string get_hostname() const;
        void        set_status(const short& status);
        void        set_nickname(const std::string& nickname);
        void        set_username(const std::string& username);
        void        set_hostname(const std::string& hostname);
		bool		is_valid_username(const std::string& user);
		bool		is_nickname_char(char c);
		bool		nick_already_used(const std::string& nick, const std::map<int, Client>& clients, const Client& current_client);

        static pollfd  create_socket(int fd, short events, short revents);

        friend class Command;
        friend class Join;
		friend class Kick;
		friend class Quit;
        friend class Pass;
};

bool    check_command(std::string& input, const std::string& command);