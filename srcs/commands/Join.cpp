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
        std::string raw_channel_name = to_join[i].first;
        std::string password = to_join[i].second;
        
        // Parse and validate the channel name
        std::string channel_name = parse_channel_name(raw_channel_name);
        
        if (channel_name.empty())
        {
            client.write(ERR_BADCHANMASK(raw_channel_name));
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

        // take channel's reference
        Channel& current_channel = server._channels[channel_name];
        
        // throw join message for this channel
        std::string join_msg = client.get_mask() + "JOIN " + channel_name;
        current_channel.broadcastEveryone(join_msg, client);
        
		//throw user list of this channel only to the client who just joined
		server.send_user_list(client, current_channel);
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
    if (channel.modes['l'] && channel.getClientCount() >= channel.usersLimit)
    {
        client.write(ERR_CHANNELISFULL(client.nickname, channel.name));
        return false;
    }

    return true;
}

std::vector<std::pair<std::string, std::string> > Join::split_join(const std::string& str)
{
	size_t space_pos = str.find(' ');
	std::string channels_part;
	std::string keys_part;
	if (space_pos == std::string::npos)
		channels_part = str;
	else
	{
		channels_part = str.substr(0, space_pos);
		keys_part     = str.substr(space_pos + 1);
	}

	std::vector<std::string> channels = split(channels_part, ",");
	std::vector<std::string> keys;
	if (!keys_part.empty())
		keys = split(keys_part, ",");

	std::vector<std::pair<std::string, std::string> > result;
	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::string trimmed = channels[i];
		trimmed.erase(0, trimmed.find_first_not_of(" \t"));
		trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

		std::string key = "";
		if (i < keys.size())
		{
			key = keys[i];
			key.erase(0, key.find_first_not_of(" \t"));
			key.erase(key.find_last_not_of(" \t") + 1);
		}

		result.push_back(std::make_pair(trimmed, key));
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

std::string Join::parse_channel_name(const std::string& input_channel)
{
    std::string channel = input_channel;
    channel.erase(0, channel.find_first_not_of(" \t"));
    channel.erase(channel.find_last_not_of(" \t") + 1);
    
    if (channel.empty())
        return "";

    if (channel.length() < 2)
        return "";

    if (channel.length() > 50)
        return "";
    
    if (channel[0] != '#')
        return "";
    
    if (channel.length() > 1 && isdigit(channel[1]))
        return "";
    
    for (size_t i = 1; i < channel.length(); ++i) {
        const char c = channel[i];
        if (!isalnum(c) && c != '-' && c != '_' && c != '.') {
            return "";
        }
    }
    
    if (channel.find(' ') != std::string::npos)
        return "";
    
    if (channel.find(',') != std::string::npos)
        return "";
    
    return channel;
}

/*
    Returns a bool indicating if the channel name is ok
*/

bool Join::is_channel_valid(const std::string& channel) 
{    
    return !parse_channel_name(channel).empty();
}