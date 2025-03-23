#include "Client.hpp"

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