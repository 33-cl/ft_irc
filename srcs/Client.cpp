#include "../includes/Client.hpp"

Client::Client() : status(UNREGISTERED) {}

Client::Client(int fd) : status(UNREGISTERED)
{
    this->socket.fd = fd;
    this->socket.events = POLLIN;
    this->socket.revents = 0;
}

Client::~Client() {}

pollfd    Client::get_socket() const
{
    return this->socket;
}

short   Client::get_status() const
{
    return this->status;
}

void    Client::set_status(const short &status)
{
    this->status = status;
}

pollfd  Client::create_socket(int fd, short events, short revents)
{
    pollfd  new_socket;

    new_socket.fd = fd;
    new_socket.events = events;
    new_socket.revents = revents;

    return new_socket;
}
