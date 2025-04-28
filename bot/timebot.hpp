#pragma once

#include "../srcs/messages.hpp"
#include <string>
#include <vector>

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


private:
    int         _sockfd;        // descriptor du socket client
    std::string _server_ip;     // argv[1]
    int         _port;          // argv[2]
    std::string _password;      // argv[3]

    std::string _nick;          // par ex. "TimeBot"
    std::string _user;          // par ex. "TimeBot 0 * :Time Bot"
    std::string _buffer;        // pour stocker le dernier recv()

    // traite une seule ligne sans CRLF
    void handle_line(const std::string &line);
	void    write(const std::string& str);
	std::vector<std::string> split(const std::string &s, const std::string &delim);
	std::vector<std::string> split_white_spaces(const std::string& str);
	
};

