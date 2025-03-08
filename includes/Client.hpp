#pragma once

#include <poll.h>
#include <arpa/inet.h>
#include <string>

enum status {
    UNREGISTERED,
    REGISTERED,
    PENDING_REGISTRATION,
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

        void    login();

        pollfd  get_socket() const;
        short   get_status() const;
        void    set_status(const short &status);

        static pollfd  create_socket(int fd, short events, short revents);
};
