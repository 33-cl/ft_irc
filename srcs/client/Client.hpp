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

        std::string get_mask();
        
        static pollfd  create_socket(int fd, short events, short revents);
        
        void    send_msg(const std::string& str);
        void    write(const std::string& str);

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
		friend class Part;
		friend class Mode;
		friend class Invite;
};

bool    check_command(std::string& input, const std::string& command);
