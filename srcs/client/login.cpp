#include "Client.hpp"

void    Client::login(const std::string& input, const std::string& password)
{
    if (status == UNREGISTERED)
    {
        if (input.size() < 6 || input.compare(0, 5, "PASS "))
            return;
        if (input.substr(5, input.length() - 5 - 1) != password) // substr() exclut le \n a la fin du buffer
            throw std::invalid_argument(":ircserv 464 * :Password incorrect\r\n");
        status = PENDING_REGISTRATION;
    }
    else if (status == PENDING_REGISTRATION)
    {
        if (input.size() > 5 && !input.compare(0, 5, "NICK "))
        {
            std::string new_nick = input.substr(5, input.length() - 5 - 1);
            
            // No nickname given
            std::cout << "|" << new_nick << "|\n";
            if (new_nick.empty())
                throw std::invalid_argument(":irc.example.com 431 * :No nickname given\r\n");
            
            // Erroneous nickname (contains invalid characters)
            for (size_t i = 0; i < new_nick.length(); i++)
            {
                if (!isalnum(new_nick[i]) && new_nick[i] != '-' && new_nick[i] != '_')
                    throw std::invalid_argument(":irc.example.com 432 * " + new_nick + " :Erroneous nickname\r\n");
            }
            
            // Check for existing nickname (this would require access to a list of existing nicknames)
            // Assuming there's a function or way to check if the nickname exists
            // if (isNickInUse(new_nick))
            //     throw std::invalid_argument(":irc.example.com 433 * " + new_nick + " :Nickname is already in use\r\n");
            
            // Nickname collision KILL
            // if (hasNickCollision(new_nick))
            //     throw std::invalid_argument(":irc.example.com 436 * " + new_nick + " :Nickname collision KILL\r\n");
            
            // Temporarily unavailable nickname
            // if (isNickReserved(new_nick))
            //     throw std::invalid_argument(":irc.example.com 437 * " + new_nick + " :Nick/channel is temporarily unavailable\r\n");
            
            // Connection restricted
            // if (isConnectionRestricted())
            //     throw std::invalid_argument(":irc.example.com 484 * " + new_nick + " :Your connection is restricted\r\n");
            
            nickname = new_nick;
            if (username != "")
                status = REGISTERED;
        }
        if (input.size() > 6 && !input.compare(0, 5, "USER "))
        {
            username = input.substr(5, input.length() - 5 - 1);
            if (nickname != "")
                status = REGISTERED;
        }
    }
}