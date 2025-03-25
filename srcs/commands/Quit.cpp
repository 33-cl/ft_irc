/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qordoux <qordoux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:33:24 by qordoux           #+#    #+#             */
/*   Updated: 2025/03/25 18:08:17 by qordoux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void   Quit::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	std::string quitMessage;
	if (args.size() <= 1 || args[1].empty())
		quitMessage = "Client disconnected";
	else
	{
		quitMessage = args[1];
		for (size_t i = 2; i < args.size(); ++i)
			quitMessage += " " + args[i];
	}
	std::string message = ":" + client.nickname + "!" + client.username + "@localhost QUIT :" + quitMessage + "\r\n";

    for (std::map<std::string, Channel>::iterator it = server._channels.begin();
         it != server._channels.end(); ++it)
    {
        Channel& channel = it->second;
        if (channel.hasClient(client.socket.fd))
        {
            channel.broadcast(message);
            channel.removeClient(client.socket.fd);
        }
    }
}
