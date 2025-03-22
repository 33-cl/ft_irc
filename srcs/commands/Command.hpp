#pragma once

#include "../client/Client.hpp"

class Client;

class Command
{
    public:
        Command();
        virtual ~Command();

        virtual void    execute(Client& client, std::vector<std::string>& args) = 0;
};

class Join : public Command
{
    public:
        Join();
        ~Join();

        void    execute(Client& client, std::vector<std::string>& args);
};
