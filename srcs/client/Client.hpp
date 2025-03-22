#pragma once

#include "../channel/Channel.hpp"

#include <poll.h>
#include <arpa/inet.h>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <algorithm>

enum status {
    UNREGISTERED,
    PENDING_REGISTRATION,
    REGISTERED,
};

class Client
{
    private:
        pollfd      socket;
        std::string nickname;
        std::string username;
        std::string hostname;
        short       status;

    public:
        Client();
        Client(int fd);
        ~Client();

        void    write(const std::string& str);

        void    login(std::string& input, const std::string& password);
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

        static pollfd  create_socket(int fd, short events, short revents);
};

bool    check_command(std::string& input, const std::string& command);