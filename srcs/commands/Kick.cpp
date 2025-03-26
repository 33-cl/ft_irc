/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maeferre <maeferre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:43:06 by qordoux           #+#    #+#             */
/*   Updated: 2025/03/26 14:58:02 by maeferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void 	Kick::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	if (client.status != REGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED("*"));

	// std::cout << "KICK COMMAND" << std::endl;
	// (void)client;
	(void)args;
	(void)server;

	client.write("KICK COMMAND\n");
}