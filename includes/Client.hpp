#pragma once

#include <poll.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>

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
        short       status;

    public:
        Client();
        Client(int fd);
        ~Client();

        void    login(const std::string& input, const std::string& password);

        pollfd      get_socket() const;
        short       get_status() const;
        std::string get_nickname() const;
        std::string get_username() const;
        void        set_status(const short& status);
        void        set_nickname(const std::string& nickname);
        void        set_username(const std::string& username);

        static pollfd  create_socket(int fd, short events, short revents);
};
