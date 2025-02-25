#pragma once

#include <iostream>
#include <utility>

class Server
{
    private:
        std::string _password;
        int         _port;

    public:
        Server(std::string password, int port);
        ~Server();
};

