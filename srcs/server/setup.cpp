#include "Server.hpp"

Server::Server() : _password(""), _port(0) {}

Server::~Server() 
{
    close(_fd);

    delete  _commands["JOIN"];
	delete  _commands["KICK"];
}

void    Server::init_commands()
{
    _commands["JOIN"] = new Join;
	_commands["KICK"] = new Kick;
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

    // Creer la socket du server
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

    init_commands();

    std::cout << "Server initialized on port " << _port << std::endl;
}

void    Server::start()
{
    std::vector<pollfd> sockets;
    sockets.push_back(Client::create_socket(_fd, POLLIN, 0));

    while (true)
    {
        infos();

        int nb_sockets = poll(sockets.data(), sockets.size(), -1);
        if (nb_sockets == -1)
            break;

        for (size_t i = 0; i < sockets.size(); i++)
        {//on met pollin dans events car c'est ce qu'on surveille, dans revent on trouve ce que poll a detecte
            if (sockets[i].revents & POLLIN)//isole juste la partie de bit pollin pour voir si il est present
            {
                if (sockets[i].fd == this->_fd)
                {
                    std::cout << "----------\nNewClient\n---------\n";
                    new_client(sockets);
                }
                else
                    process_client_data(sockets, i);
            }
        }
    }
}
