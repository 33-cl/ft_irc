#include "Command.hpp"

Pass::Pass() {}

Pass::~Pass() {}

void    Pass::execute(Client& client, std::vector<std::string>& args, Server& server)
{
    if (client.status != UNREGISTERED)
        return; // a gerer :server 462 <nickname> :You may not reregister

    if (args[0] != "PASS")
        return;

    if (args.size() != 2)
        return; // a gerer


    if (args[1] != server._password)
        throw std::invalid_argument(":ircserv 464 * :Password incorrect\r\n");

    client.status = PENDING_REGISTRATION;
}