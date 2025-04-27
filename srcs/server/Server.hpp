#pragma once

#include "../client/Client.hpp"
#include "../channel/Channel.hpp"
#include "../commands/Command.hpp"
#include "../messages.hpp"

#include <iostream>
#include <sstream>
#include <utility>
#include <cstdlib>
#include <vector>
#include <map>
#include <csignal>

#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <string.h>

class Command;
class Channel;
class Client;

class Server
{
	friend class Client;
    private:
        bool                            _is_running;
        std::string                     _password;
        int                             _port;
        int                             _fd;
        std::map<int, Client>           _clients;
        std::map<std::string, Channel>  _channels;
        std::map<std::string, Command*> _commands;
		std::string                     _name;

    public:
        Server();
        ~Server();

        static Server&  get_server();

        Client& find_client(const std::string& nickname);

        void    init_commands();
        void    initialize(int argc, char **argv);
        void    start();
        
        void    new_client(std::vector<pollfd>& fds);
        void    remove_client(const Client& client, const std::string& message);
        void    process_client_data(std::vector<pollfd>& fds, int client_index);
        void    process_input(std::string& input, Client& client);
        void    create_channel(const std::string& channel_name, Client& client);
		int		getFdByNickname(const std::string &nickname) const;

        void    infos();

        static void handle_signal(int signal);

        friend class Client;
        friend class Channel;
        friend class Command;
        friend class Pass;
        friend class Nick;
        friend class User;
        friend class Join;
        friend class Privmsg;
        friend class Notice;
        friend class Mode;
        friend class Kick;
        friend class Topic;
        friend class Quit;
		friend class Part;
		friend class Mode;
		friend class Invite;
        friend class List;
};

template <typename T>
std::string toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::vector<std::string>    split(const std::string& str, const std::string& delimiter = "\r\n");
std::vector<std::string>	split_white_spaces(const std::string& str);
std::string                 unsplit(const std::vector<std::string>& tab) ;
bool                        check_command(std::string& input, const std::string& command);
bool                        is_password_valid(const std::string& password);