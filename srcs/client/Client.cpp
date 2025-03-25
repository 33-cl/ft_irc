#include "Client.hpp"

Client::Client() : nickname(""), username(""), status(UNREGISTERED) {}

Client::Client(int fd) : nickname(""), username(""), status(UNREGISTERED)
{
    this->socket.fd = fd;
    this->socket.events = POLLIN;
    this->socket.revents = 0;
}

Client::~Client() {}

pollfd  Client::create_socket(int fd, short events, short revents)
{
    pollfd  new_socket;

    new_socket.fd = fd;
    new_socket.events = events;
    new_socket.revents = revents;

    return new_socket;
}

/*
    Sends a string to the client
*/

void    Client::send_msg(const std::string& str)
{
    std::string error_msg = ":" + std::string(SERVER_NAME) + " " + str + "\r\n";
    this->write(error_msg);
}

void    Client::write(const std::string& str) 
{
    std::string message = str + "\r\n";
    ssize_t bytes_sent = send(socket.fd, message.c_str(), message.length(), 0);
    if (bytes_sent == -1)
        std::cerr << "Error while sending data" << std::endl;
}
