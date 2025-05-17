#include "Server.hpp"

Server::Server() : _password(""), _port(0), _name("ircserv") {}

Server::Server(int argc, char **argv)
{
    _name = "ircserv";

    if (argc != 3)
        throw critical_error("Invalid nb of args\nusage: ./ircserv <port> <password>");
    
    std::string port_str = argv[1];
    if (port_str.find_first_not_of("0123456789") != std::string::npos)
        throw critical_error("Port must be numeric");

    _port = atoi(port_str.c_str());
    if (_port <= 0 || _port > 65535)
        throw critical_error("Port must be between 1 and 65535");

    _password = argv[2];

    if (!is_password_valid(_password))
        throw critical_error("Invalid password syntax");

    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
        throw critical_error("socket() error");

    if (!set_non_blocking(_fd))
        throw critical_error("Failed to set server socket to non-blocking mode");

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(_port);
    if (bind(_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		close(_fd);
        throw critical_error("bind() error");
	}

    if (listen(_fd, 128) == -1)
	{
		close(_fd);
        throw critical_error("listen() error");
	}

    init_commands();

    signal(SIGINT, &Server::handle_signal);
    signal(SIGQUIT, &Server::handle_signal);

    _is_running = true;
    std::cout << "Server initialized on port " << _port << std::endl;
}

void    Server::init_commands()
{
    _commands["PASS"]    = new Pass;
    _commands["NICK"]    = new Nick;
    _commands["USER"]    = new User;
    _commands["JOIN"]    = new Join;
    _commands["PRIVMSG"] = new Privmsg;
	_commands["MODE"]    = new Mode;
    _commands["KICK"]    = new Kick;
    _commands["TOPIC"]   = new Topic;
    _commands["QUIT"]    = new Quit;
	_commands["PART"]    = new Part;
	_commands["INVITE"]  = new Invite;
    _commands["LIST"]    = new List;
    _commands["NOTICE"]  = new Notice;
}

void Server::start()
{
    std::vector<pollfd> sockets;
    sockets.push_back(Client::create_socket(_fd, POLLIN, 0));

    while (_is_running)
    {
        // infos();

        int nb_sockets = poll(sockets.data(), sockets.size(), -1);
        if (nb_sockets == -1)
            break;

        for (size_t i = 0; i < sockets.size(); )
        {
            if (sockets[i].revents & POLLIN)
            {
                if (sockets[i].fd == this->_fd)
                {
                    new_client(sockets);
                    i++;
                }
                else
                {
                    try
                    {
                        process_client_data(sockets, i);
                        i++;
                    }
                    catch(const quit_server& e)
                    {
                        std::cerr << "Client disconnected: " << e.what() << '\n';
                        
                        close(sockets[i].fd);
                        _clients.erase(sockets[i].fd);
                        sockets.erase(sockets.begin() + i);
                    }
                }
            }
            else
                i++;
        }
    }
    delete_clients();
}

Server::~Server() 
{
    close(_fd);

    delete  _commands["PASS"]; 
    delete  _commands["NICK"];   
    delete  _commands["USER"];   
    delete  _commands["JOIN"];   
    delete  _commands["PRIVMSG"];
    delete  _commands["KICK"];   
    delete  _commands["TOPIC"];
    delete  _commands["QUIT"]; 
	delete  _commands["PART"];
	delete  _commands["MODE"];
	delete  _commands["INVITE"];
    delete  _commands["LIST"];
    delete  _commands["NOTICE"];
}