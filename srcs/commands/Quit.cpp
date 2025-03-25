/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qordoux <qordoux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:33:24 by qordoux           #+#    #+#             */
/*   Updated: 2025/03/25 17:00:22 by qordoux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void   Quit::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	std::string quitMessage = (args.empty() ? "Client disconnected" : args[0]);
	std::string message = ":" + client.nickname + " QUIT :" + quitMessage;

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
