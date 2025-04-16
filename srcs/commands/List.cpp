#include "Command.hpp"

List::List() {}

List::~List() {}

void List::execute(Client& client, std::vector<std::string>& args, Server& server) 
{
    (void)args;

    client.send_msg("321 " + client.nickname + " Channel :Users  Name");

    const std::map<std::string, Channel>& channels = server._channels;
    std::map<std::string, Channel>::const_iterator it;

    for (it = channels.begin(); it != channels.end(); ++it) 
    {
        const std::string& channelName = it->first;
        const Channel& channel = it->second;

        std::ostringstream oss;
        oss << "322 " << client.nickname << " " 
            << channelName << " " << channel.clients.size() << " :" << channel.topic;
        client.send_msg(oss.str());
    }

    client.send_msg("323 " + client.nickname + " :End of /LIST");
}