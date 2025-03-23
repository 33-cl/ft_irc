#include "Command.hpp"

Pass::Pass() {}

Pass::~Pass() {}

/*
    Erreurs non gerees
        - args.size() != 2
*/

void    Pass::execute(Client& client, std::vector<std::string>& args, Server& server)
{
    if (args[0] != "PASS")
        return;

    if (args.size() != 2)
        return; // a gerer


    if (args[1] != server._password)
        throw std::invalid_argument(":ircserv 464 * :Password incorrect\r\n");

    client.status = PENDING_REGISTRATION;
}