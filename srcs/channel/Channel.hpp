#pragma once

// #include "../messages.hpp"

#include "../client/Client.hpp"

#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

class Client;
class Server;


class Channel
{
    private:
		std::string				name;
		std::vector<Client>		clients; // Stocking clients ids
		std::string				topic;
		std::vector<Client> 	invites;
		std::vector<Client> 	operators;
		std::map<char, bool>	modes;
		int						usersLimit;
		std::string				modesStr;
		std::string				password;


    public:
        Channel();
        Channel(std::string name, const Client& client);
        ~Channel();

        void    add_client(Client new_client);
		bool	hasClient(int fd) const;
		void	removeClient(int fd);
		void	addInvite(const Client& client);
		void	removeInvite(const Client& client);
		void	addOperator(const Client& client);
		void	removeOperator(const Client& client);

		bool			isOperator(const Client& client) const;
		bool			isInvited(const Client& client) const;
		std::string		getModesString() const;
		bool			changeMode(const std::string &modeChanges, const std::vector<std::string>& modeParams, Client& client, Server& server);
		int				getClientCount(std::vector<Client> clients);


        void    broadcast(const std::string& str, const Client& src);

        std::vector<Client>    get_clients() const;  

        friend class Server;
        friend class Client;
        friend class Command;
        friend class Join;
		friend class Mode;
		friend class Invite;
		friend class Topic;
		friend class List;
};
