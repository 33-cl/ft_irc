#include "../includes/Client.hpp"

Client::Client() : nickname(""), username(""), status(UNREGISTERED) {}

Client::Client(int fd) : nickname(""), username(""), status(UNREGISTERED)
{
    this->socket.fd = fd;
    this->socket.events = POLLIN;
    this->socket.revents = 0;
}

Client::~Client() {}

/*
    Erreurs de login a implementer

    Tentative de changement de pseudonyme sans fournir de pseudonyme
    NICK
    :irc.example.com 431 * :No nickname given

    Tentative de changement de pseudonyme avec un pseudonyme invalide
    NICK Bad!Nick
    :irc.example.com 432 * Bad!Nick :Erroneous nickname

    Tentative de changement de pseudonyme avec un pseudonyme déjà utilisé
    NICK ExistingNick
    :irc.example.com 433 * ExistingNick :Nickname is already in use

    Tentative de changement de pseudonyme avec un pseudonyme en collision
    NICK CollidingNick
    :irc.example.com 436 * CollidingNick :Nickname collision KILL

    Tentative de changement de pseudonyme avec un pseudonyme temporairement indisponible
    NICK ReservedNick
    :irc.example.com 437 * ReservedNick :Nick/channel is temporarily unavailable

    Tentative de changement de pseudonyme avec une connexion restreinte
    NICK NewNick
    :irc.example.com 484 * NewNick :Your connection is restricted
*/

void    Client::login(const std::string& input, const std::string& password)
{
    if (status == UNREGISTERED)
    {
        if (input.size() < 6 || input.compare(0, 5, "PASS "))
            return;
        if (input.substr(5, input.length() - 5 - 1) != password) // substr() exclut le \n a la fin du buffer
            throw   std::invalid_argument(":ircserv 464 * :Password incorrect\r\n");
        status = PENDING_REGISTRATION;
    }
    else if (status == PENDING_REGISTRATION)
    {
        if (input.size() > 6 && !input.compare(0, 5, "NICK "))
        {
            nickname = input.substr(5, input.length() - 5 - 1);
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

pollfd    Client::get_socket() const
{
    return this->socket;
}

short   Client::get_status() const
{
    return this->status;
}

std::string Client::get_nickname() const
{
    return this->nickname;
}

std::string Client::get_username() const
{
    return this->username;
}

void    Client::set_status(const short& status)
{
    this->status = status;
}

void    Client::set_nickname(const std::string& nickname)
{
    this->nickname = nickname;
}

void    Client::set_username(const std::string& username)
{
    this->username = username;
}

pollfd  Client::create_socket(int fd, short events, short revents)
{
    pollfd  new_socket;

    new_socket.fd = fd;
    new_socket.events = events;
    new_socket.revents = revents;

    return new_socket;
}
