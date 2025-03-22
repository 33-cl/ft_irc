#include "Command.hpp"

Join::Join() {}

Join::~Join() {}

void    Join::execute(Client& client, std::vector<std::string>& args)
{
    std::cout << "Join function called" << std::endl;
    std::string join = "Join function called";
    client.write(join);
    (void)args;
}
