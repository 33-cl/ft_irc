#include "Client.hpp"

Client::Client() : nickname(""), username(""), status(UNREGISTERED) {}

Client::Client(int fd) : nickname(""), username(""), status(UNREGISTERED)
{
    this->socket.fd = fd;
    this->socket.events = POLLIN;
    this->socket.revents = 0;
}

pollfd  Client::create_socket(int fd, short events, short revents)
{
    pollfd  new_socket;

    new_socket.fd = fd;
    new_socket.events = events;
    new_socket.revents = revents;

    return new_socket;
}
