#include "Client.hpp"

Client::Client() : nickname(""), username(""), status(UNREGISTERED) {}

Client::Client(int fd) : nickname(""), username(""), status(UNREGISTERED)
{
    this->socket.fd = fd;
    this->socket.events = POLLIN;
    this->socket.revents = 0;
}

Client::~Client() 
{
}

std::string Client::get_mask()
{
    std::string mask = ":" + nickname + "!" + username + "@localhost ";
    return mask;
}

pollfd  Client::create_socket(int fd, short events, short revents)
{
    pollfd  new_socket;

    new_socket.fd = fd;
    new_socket.events = events;
    new_socket.revents = revents;

    return new_socket;
}

/*
    Sends a string to the client
*/

void    Client::send_msg(const std::string& str)
{
    std::string error_msg = ":" + std::string(SERVER_NAME) + " " + str;
    this->write(error_msg);
}

void    Client::write(const std::string& str) 
{
	std::string message = str + "\r\n";
	ssize_t bytes_sent = send(socket.fd, message.c_str(), message.length(), MSG_NOSIGNAL | MSG_DONTWAIT);
	if (bytes_sent == -1)
	{
		std::cerr << "Error while sending data" << std::endl;
		Server& serv = Server::get_server();
		serv.remove_client(*this, "");  
		return;
	}
}

void Client::append_to_buffer(const std::string& data) {
    input_buffer += data;
}

std::string Client::get_buffer() {
    return input_buffer;
}

void Client::clear_buffer() {
    input_buffer.clear();
}