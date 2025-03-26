/*
    GESTION D'ERREURS

    1.Erreurs récupérables
        Exemples : Mot de passe incorrect, commande mal formatée, canal inexistant.
        Traitement : try -> catch, envoyer un message d'erreur via send(), permettre au client de réessayer.

    2.Erreurs critiques
        Exemples : Défaillance du serveur, erreur de mémoire, problème réseau.
        Traitement : try -> catch, journaliser l'erreur, fermer la connexion ou redémarrer, ne pas permettre de réessayer immédiatement.

    3.Erreurs de validation
        Exemples : Pseudonyme déjà utilisé, canal plein, tentative non autorisée.
        Traitement : Envoyer un message d'erreur via send(), bloquer l'action, permettre une nouvelle tentative avec des données valides.

    En bref :
    Récupérables : Client peut réessayer.
    Critiques : Problème grave, fermer ou redémarrer.
    Validation : Bloquer l'action, demander une correction.

*/

#pragma once

#include <iostream>
#include <utility>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <map>
#include <poll.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_NAME "ircserv"

#define RPL_WELCOME(nick, mask)                     ("001 " + std::string(nick) + " Welcome to the IRC Network " + std::string(mask))
#define ERR_NOSUCHNICK(client, nick)                ("401 " + std::string(client) + " " + std::string(nick) + " :No such nick/channel")
#define ERR_NOSUCHCHANNEL(client, channel)          ("403 " + std::string(client) + " " + std::string(channel) + " :No such channel")
#define ERR_NONICKNAMEGIVEN(nick)                   ("431 " + std::string(nick) + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(nick)                  ("432 " + std::string(nick) + " :Erroneous nickname")
#define ERR_NICKNAMEINUSE(nick)                     ("433 " + std::string(nick) + " " + std::string(nick) + " :Nickname is already in use")
#define ERR_USERNOTINCHANNEL(client, user, channel) ("441 " + std::string(client) + " " + std::string(user) + " " + std::string(channel) + " :They aren't on that channel")
#define ERR_NOTONCHANNEL(client, channel)           ("442 " + std::string(client) + " " + std::string(channel) + " :You're not on that channel")
#define ERR_USERONCHANNEL(nick, channel)            ("443 " + std::string(nick) + " " + std::string(channel) + " :is already on channel")
#define ERR_NOTREGISTERED(nick)                     ("451 " + std::string(nick) + " :You have not registered")
#define ERR_NEEDMOREPARAMS(nick, cmd)               ("461 " + std::string(nick) + " " + std::string(cmd) + " :Not enough parameters")
#define ERR_ALREADYREGISTERED(nick)                 ("462 " + std::string(nick) + " :You may not reregister")
#define ERR_PASSWDMISMATCH(nick)                    ("464 " + std::string(nick) + " :Password incorrect")
#define ERR_BADCHANMASK(channel)                    ("476 " + std::string(channel) + " :Bad channel mask")

class critical_error : public std::exception
{
    private:
        std::string message;
    
    public:
        critical_error(const std::string& str) : message(str) {}
        virtual ~critical_error() throw() {}

        virtual const char* what() const throw()
        {
            return message.c_str();
        }
};

class recoverable_error : public std::exception
{
    private:
        std::string message;
    
    public:
        recoverable_error(const std::string& str) : message(str) {}
        virtual ~recoverable_error() throw() {}

        virtual const char* what() const throw()
        {
            return message.c_str();
        }
};
