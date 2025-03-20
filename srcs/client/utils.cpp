#include "Client.hpp"

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

bool is_command_valid(std::string& input, const std::string& command) {
    if (input[input.size() - 1] == '\n') // With some clients, the input be followed by '\n'
        input.erase(input.size() - 1);   // so we remove it the handle the inputs equally

    // Check is the input size is correct and starting by a command followed by a space
    if (input.size() < command.size() + 2 || input.compare(0, command.size() + 1, command + " ") != 0) {
        return false;
    }

    return true;
}