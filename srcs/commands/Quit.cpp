/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qordoux <qordoux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:33:24 by qordoux           #+#    #+#             */
/*   Updated: 2025/03/23 20:57:18 by qordoux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void   Quit::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	client.write("QUIT COMMAND\n");
	
	//message de deconnection ou non
	std::string quitMessage = (args.empty() ? "Client disconnected" : args[0]);

	std::string message = ":" + client.get_nickname() + " QUIT :" + quitMessage;

	  // Parcourir la map des channels du serveur avec un itérateur classique
    for (std::map<std::string, Channel>::iterator it = server._channels.begin();
         it != server._channels.end(); ++it)
    {
        Channel& channel = it->second;
        // Vérifier si le client est présent dans ce canal
        if (channel.hasClient(client.socket.fd))
        {
            // Diffuser le message de déconnexion aux autres membres du canal
            // channel.broadcast(message, client);
            // // Retirer le client du canal
            // channel.removeClient(client);
        }
    }
}