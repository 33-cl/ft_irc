#include "../includes/Client.hpp"

Client::Client() {}

Client::Client(int fd)
{
    this->socket.fd = fd;
    this->socket.events = POLLIN;
    this->socket.revents = 0;
}

Client::~Client() {}

const pollfd    Client::get_socket()
{
    return this->socket;
}
pollfd  Client::create_socket(int fd, short events, short revents)
{
    pollfd  new_socket;

    new_socket.fd = fd;
    new_socket.events = events;
    new_socket.revents = revents;

    return new_socket;
}
