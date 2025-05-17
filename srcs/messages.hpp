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

//Replies:
#define RPL_WELCOME(nick, mask)                     ("001 " + std::string(nick) + " Welcome to the IRC Network " + std::string(mask))
#define RPL_CHANNELMODEIS(nickname, channel, modes)	("324 " + std::string(nickname) + " " + std::string(channel) + " " + std::string(modes))
#define RPL_NOTOPIC(nick, channel)					("331 " + std::string(nick) + " " + std::string(channel) + " :No topic is set")
#define RPL_TOPIC(nick, channel, topic)				("332 " + std::string(nick) + " " + std::string(channel) + " :" + std::string(topic))
#define RPL_INVITING(nickname, channel, inviter)	("341 " + std::string(nickname) + " " + std::string(target) + " " + std::string(channel))

//Error:
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
#define ERR_TOOMANYPARAMS(nick, command)	        ("461 " + std::string(nick) + " " + std::string(command) + " :Too many parameters")
#define ERR_INVALIDMODESTRING(nick)					("461 " + std::string(nick) + " MODE :Mode string must contain mode flags")
#define ERR_MODE_SPACES(nick)						("461 " + std::string(nick) + " MODE :Mode string must not contain spaces")
#define ERR_MISMATCHMULTIKICK(client)				("461 " + std::string(client) + " :Number of channels and targets do not match for multi-kick")
#define ERR_ALREADYREGISTERED(nick)                 ("462 " + std::string(nick) + " :You may not reregister")
#define ERR_PASSWDMISMATCH(nick)                    ("464 " + std::string(nick) + " :Password incorrect")
#define ERR_INVALIDQUITMESSAGE(nick)		        ("465 " + std::string(nick) + " :Invalid QUIT message format, comment must begin with ':'")
#define ERR_INVALIDKICKMESSAGE(nick)		        ("465 " + std::string(nick) + " :Invalid KICK message format, comment must begin with ':'")
#define ERR_INVALIDUSERFORMAT(nick)					("465 " + std::string(nick) + " :Invalid USER command format, real name must begin with ':'")
#define ERR_INVALIDPARTMESSAGE(nick)				("465 " + std::string(nick) + " :Invalid PART message format, reason must begin with ':'")
#define ERR_CHANNELISFULL(client, channel)			("471 " + std::string(client) + " " + std::string(channel) + " :Cannot join channel (+l)")
#define ERR_UNKNOWNMODE(nick, channel)				("472 " + std::string(nick) + " " + std::string(channel) + " :is unknown mode char to me")
#define ERR_INVALIDMODEFLAG(client)					("472 " + std::string(client) + " :Mode flag must begin with '+' or '-'")
#define ERR_INVALIDUSERLIMIT(client)				("472 " + std::string(client) + " :Invalid user limit value")
#define ERR_EMPTYKEY(client)						("472 " + std::string(client) + " :Empty key parameter")
#define ERR_NOTINVITED(nick, channel)				("473 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(nick, channel)			("475 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+k)")
#define ERR_BADCHANMASK(channel)                    ("476 " + std::string(channel) + " :Bad channel mask")
#define ERR_CANNOTKICKSELF(nick)			        ("480 " + std::string(nick) + " :You cannot kick yourself")
#define ERR_CANNOTINVITESELF(nick)					("480 " + std::string(nick) + " :You cannot invite yourself")
#define ERR_NOTCHANNELOP(nick, channel)				("482 " + std::string(nick) + " " + std::string(channel) + " :You're not channel operator")
#define ERR_ALREADYOP(client, nick)					("482 " + std::string(client) + " " + std::string(nick) + " :is already an operator")
#define ERR_NOTANOPERATOR(client, nick)				("482 " + std::string(client) + " " + std::string(nick) + " :is not an operator")
#define ERR_OPERATORNOTFOUND(client, nick)			("484 " + std::string(client) + " " + std::string(nick) + " :Operator not found")
#define ERR_UMODEUNKNOWNFLAG(client)				("501 " + std::string(client) + " :User mode changes not implemented")
#define ERR_NOSUCHNICK_DETAILED(client, nick)     ("401 " + std::string(client) + " " + std::string(nick) + " :No such nickname")


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

class quit_server : public std::exception
{
    private:
        std::string message;
    
    public:
        quit_server(const std::string& str) : message(str) {}
        virtual ~quit_server() throw() {}

        virtual const char* what() const throw()
        {
            return message.c_str();
        }
};
