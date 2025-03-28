#include "Command.hpp"

Pass::Pass() {}

Pass::~Pass() {}

void    Pass::execute(Client& client, std::vector<std::string>& args, Server& server)
{
    if (client.status != UNREGISTERED)
        throw recoverable_error(ERR_ALREADYREGISTERED(client.nickname));

    if (args.size() < 2)
        throw recoverable_error(ERR_NEEDMOREPARAMS("*", "PASS"));
    if (args.size() > 2)
        throw recoverable_error(ERR_TOOMANYPARAMS("*", "PASS"));

    if (args[1] != server._password)
        throw recoverable_error(ERR_PASSWDMISMATCH("*"));

    client.status = PENDING_REGISTRATION;
}