#include "Server.hpp"

void Server::handle_signal(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		std::string message = "Signal received (" + toString(signal) + ")";
        Server& server = Server::get_server();
		server._is_running = false;
	}
}
