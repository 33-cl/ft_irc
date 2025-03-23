/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qordoux <qordoux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:43:06 by qordoux           #+#    #+#             */
/*   Updated: 2025/03/23 19:48:35 by qordoux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void 	Kick::execute(Client& client, std::vector<std::string>& args, Server& server)
{
	// std::cout << "KICK COMMAND" << std::endl;
	// (void)client;
	(void)args;
	(void)server;

	client.write("KICK COMMAND\n");
}