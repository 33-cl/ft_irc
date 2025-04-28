#pragma once

#include "../srcs/client/Client.hpp"
#include <string>
#include <ctime>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//besoin de tous ces includes?
class TimeBot
{
	public:

		//3 args pour le constru
		TimeBot(const std::string &server, int port, const std::string &pass);

		void run();

	private:

		void writeBot(const std::string &line);

		std::string formatTime() const;

		int			sockfd_;    // Socket descriptor
		std::string	server_;    // Server address
		int			port_;      // Server port
		std::string	pass_;
		std::string	nick_;
		std::string	user_;
		std::string	buffer_;    // Buffer for incoming data
};
