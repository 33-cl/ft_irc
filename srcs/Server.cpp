#include "../includes/Server.hpp"

Server::Server() : _password(""), _port(0) {}

Server::~Server() 
{
    close(_fd);
}

void    Server::initialize(int argc, char **argv)
{
    if (argc != 3)
        throw std::invalid_argument("Invalid nb of args\nusage: ./ircserv <port> <password>");
    
    std::string port_str = argv[1];
    if (port_str.find_first_not_of("0123456789") != std::string::npos)
        throw std::invalid_argument("Port must be numeric");

    _port = atoi(port_str.c_str());
    if (_port <= 0 || _port > 65535)
        throw std::invalid_argument("Port must be between 1 and 65535");

    _password = argv[2];

    // Creer la socket
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
        throw std::runtime_error("socket() error");

    // Lier la socket a l'adresse et au port
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;           // Using ipv4
    server_addr.sin_addr.s_addr = INADDR_ANY;   // Accept connexions on every available network interface
    server_addr.sin_port = htons(_port);        // Converting port 
    if (bind(_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        throw std::runtime_error("bind() error");

    // Mettre en mode ecoute
    if (listen(_fd, 5) == -1)
        throw std::runtime_error("listen() error");

    std::cout << "Server initialized on port " << _port << std::endl;
}

void    Server::new_client(std::vector<pollfd> &fds)
{
    sockaddr_in client_addr;
    socklen_t   client_len = sizeof(client_addr);
    int         client_fd;

    client_fd = accept(_fd, (sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
        return;

    std::cout << "Client connecte\n";

    Client  new_client(client_fd);

    _clients[client_fd] = new_client;
    fds.push_back(new_client.get_socket());
}

void    Server::process_client_data(std::vector<pollfd> &fds, int i)
{
    // Message d'un client existant
    char    buffer[1024];
    ssize_t bytes_received = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0)
    {
        // Afficher le message recu
        buffer[bytes_received] = '\0';
        process_input(buffer, _clients[fds[i].fd]);
    }
    else
    {
        // Supprimer le client deconnecte
        std::cout << "Client deconnecte\n";
        close(fds[i].fd);
        fds.erase(fds.begin() + i);
        i--;
    }
}

void    Server::process_input(char *input, Client &client)
{
    if (client.get_status() == UNREGISTERED)
        client.set_status(REGISTERED);
    else
        std::cout << client.get_status() << input << std::endl;
}

void    Server::start()
{
    std::vector<pollfd> sockets;
    sockets.push_back(Client::create_socket(_fd, POLLIN, 0));

    while (true)
    {
        int nb_sockets = poll(sockets.data(), sockets.size(), -1);
        if (nb_sockets == -1)
            break;

        for (size_t i = 0; i < sockets.size(); i++)
        {
            if (sockets[i].revents & POLLIN)
            {
                if (sockets[i].fd == _fd)
                    new_client(sockets);
                else
                    process_client_data(sockets, i);
            }
        }
    }
}

void    Server::infos()
{
    std::cout << "port     : " << _port     << std::endl
              << "password : " << _password << std::endl;
}
