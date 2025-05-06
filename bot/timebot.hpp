#pragma once

#include "../srcs/messages.hpp"
#include <string>
#include <vector>
#include <csignal>

class TimeBot
{
public:
    TimeBot() : _sockfd(-1), _port(0) {}

    /**  
     * Doit être appelé depuis main(int, char**).  
     * usage: timebot <server_ip> <port> <password>  
     */
    void initialize(int argc, char **argv);

    /**  
     * Envoie PASS/NICK/USER puis boucle recv/process  
     */
    void start();
	void stop();
	static TimeBot *instance;
	int getSockfd() const;



private:
	int			_sockfd;
	std::string	_server_ip;
	int			_port;
	std::string	_password;
	std::string	_nick;
	std::string	_user;
	std::string	_buffer;
	bool		_running;

	// traite une seule ligne sans CRLF
	void handle_line(const std::string &line);
	void    write(const std::string& str);
	std::vector<std::string> split(const std::string &s, const std::string &delim);
	std::vector<std::string> split_white_spaces(const std::string& str);

};

