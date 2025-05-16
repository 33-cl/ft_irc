#pragma once

#include "../srcs/messages.hpp"
#include <string>
#include <vector>
#include <csignal>

class TimeBot
{
public:
    TimeBot() : _sockfd(-1), _port(0) {}

    void initialize(int argc, char **argv);

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

	void handle_line(const std::string &line);
	void    write(const std::string& str);
	std::vector<std::string> split(const std::string &s, const std::string &delim);
	std::vector<std::string> split_white_spaces(const std::string& str);

};

