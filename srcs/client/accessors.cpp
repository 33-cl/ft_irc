#include "Client.hpp"

Client::~Client() {}

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

std::string Client::get_hostname() const
{
    return this->hostname;
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

void    Client::set_hostname(const std::string& hostname)
{
    this->hostname = hostname;
}