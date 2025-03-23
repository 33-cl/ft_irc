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

void Client::write(const std::string& str) 
{
    if (socket.fd == -1) {
        std::cerr << "Erreur : le socket est invalide." << std::endl;
        return;
    }

    std::cout << str << std::endl;

    ssize_t bytes_sent = send(socket.fd, str.c_str(), str.length(), 0);
    if (bytes_sent == -1) {
        std::cerr << "Erreur lors de l'envoi des données" << std::endl;
    }
}
