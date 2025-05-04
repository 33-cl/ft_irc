#include "Command.hpp"

Join::Join() {}

Join::~Join() {}

void Join::execute(Client& client, std::vector<std::string>& args, Server& server)
{
    if (client.status != REGISTERED)
        throw recoverable_error(ERR_NOTREGISTERED("*"));

    std::string unsplited_command = unsplit(args);
    std::vector<std::pair<std::string, std::string> > to_join = split_join(unsplited_command);

    for (size_t i = 0; i < to_join.size(); i++)
    {
        std::string channel_name = to_join[i].first;
        std::string password = to_join[i].second;

        if (!is_channel_valid(channel_name))
        {
            client.write(ERR_BADCHANMASK(channel_name));
            continue;
        }

        // Create and join
        if (server._channels.find(channel_name) == server._channels.end())
        {    
            server._channels.insert(std::pair<std::string, Channel>(channel_name, Channel(channel_name, client)));
            std::cout << "Channel " << channel_name << " created by " << client.nickname << std::endl;
        }
        // Just join
        else
        {
            Channel& channel = server._channels[channel_name];

            if (!can_join(client, channel, channel_name, password))
                continue;

            server._channels[channel_name].add_client(client);
        }


        std::string join_msg = client.get_mask() + "JOIN " + channel_name;
        client.write(join_msg);
        server._channels[channel_name].broadcast(join_msg, client);

        // Channel& channel = server._channels[channel_name];
        // std::string user_list;
        // const std::vector<Client>& clients = channel.get_clients();
        // for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        //     if (!user_list.empty())
        //         user_list += " ";
        //     user_list += it->nickname;
        // }
        // client.write(":server 353 " + client.nickname + " = " + channel_name + " :" + user_list);
        // client.write(":server 366 " + client.nickname + " " + channel_name + " :End of /NAMES list");
        server.broadcast_channel_lists();
    }
}






bool    Join::can_join(Client& client, Channel& channel, const std::string& channel_name, const std::string& password)
{
    if (is_in_channel(client, channel))
    {
        client.write(ERR_USERONCHANNEL(client.nickname, channel_name));
        return false;
    }

    // If client should be and is not invited
    if (channel.modes['i'] && !channel.isInvited(client))
    {
        client.write(ERR_NOTINVITED(client.nickname, channel_name));
        return false;
    }

    // Check password if mode 'k' is set
    if (channel.modes['k'] && password != channel.password)
    {
        client.write(ERR_BADCHANNELKEY(client.nickname, channel_name));
        return false;
    }

    // Channel is full
    if (channel.modes['l'] && channel.getClientCount(channel.clients) >= channel.usersLimit)
    {
        client.write(ERR_CHANNELISFULL(client.nickname, channel.name));
        return false;
    }

    return true;
}

std::vector<std::pair<std::string, std::string> > Join::split_join(const std::string& str)
{
    std::vector<std::pair<std::string, std::string> > result;
    std::vector<std::string> channels = split(str, ",");
    
    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string trimmed = channels[i];
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t") + 1);
        
        // Count spaces
        size_t space_count = 0;
        for (size_t j = 0; j < trimmed.size(); ++j)
        {
            if (trimmed[j] == ' ')
                space_count++;
        }
        
        if (space_count > 1)
            throw recoverable_error("Multiple spaces in channel specification");
        
        std::string channel;
        std::string key = "";
        
        size_t space_pos = trimmed.find(' ');
        if (space_pos != std::string::npos)
        {
            channel = trimmed.substr(0, space_pos);
            key = trimmed.substr(space_pos + 1);
        }
        else
            channel = trimmed;
        
        result.push_back(std::make_pair(channel, key));
    }
    return result;
}

/*
    Returns a bool indicating if the client is already in the channel
*/

bool Join::is_in_channel(Client& client, Channel& channel)
{
    const std::vector<Client>& clients = channel.get_clients();
    for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->socket.fd == client.socket.fd)
            return true;
    }
    return false;
}

/*
    Returns a bool indicating if the channel name is ok
*/

bool Join::is_channel_valid(const std::string& channel) 
{    
    if (channel.empty() || channel.length() > 50)
        return false;

    if (channel[0] != '#')
        return false;

    if (channel.length() > 1 && isdigit(channel[1]))
        return false;

    for (size_t i = 1; i < channel.length(); ++i) {
        const char c = channel[i];
        if (!isalnum(c) && c != '-' && c != '_' && c != '.') {
            return false;
        }
    }

    if (channel.find(' ') != std::string::npos)
        return false;

    return true;
}