#include "timebot.hpp"

void TimeBot::stop()
{
    _running = false;
}
TimeBot* TimeBot::instance = NULL;

int TimeBot::getSockfd() const
{
	return _sockfd;
}

void sigint_handler(int)
{
	if (TimeBot::instance)
	shutdown(TimeBot::instance->getSockfd(), SHUT_RDWR);//desactive le socket, recv revient avec 0
	TimeBot::instance->stop();
}

int main(int argc, char **argv)
{
	std::signal(SIGINT, sigint_handler);
	TimeBot bot;
	try
	{
		bot.initialize(argc, argv);
		bot.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}

void TimeBot::initialize(int argc, char **argv)
{
	if (argc != 4)
		throw std::runtime_error("Usage: timebot <server_ip> <port> <password>");

	_server_ip = argv[1];
	std::string port_str = argv[2];
	if (port_str.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("Port must be numeric");

	_port = std::atoi(port_str.c_str());
	if (_port <= 0 || _port > 65535)
		throw std::runtime_error("Port must be between 1 and 65535");

	_password = argv[3];

	// creation du socket client
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0)
		throw std::runtime_error("socket() error");

	TimeBot::instance = this;
	// remplissage de sockaddr_in
	struct sockaddr_in serv;
	// memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port   = htons(_port);
	if (inet_pton(AF_INET, _server_ip.c_str(), &serv.sin_addr) <= 0)
		throw std::runtime_error("Invalid server IP");

	// connexion au serveur IRC
	if (connect(_sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0)
		throw std::runtime_error("connect() error");

	// donnees d’enregistrement
	_nick = "TimeBot";
	_user = "TimeBot 0 * :Time Bot";

	std::cout << "TimeBot initialised for "
				<< _server_ip << ":" << _port << "\n";
}

// --- start, calque sur Server::start() mais mono-socket ---
void TimeBot::start()
{
	write("PASS " + _password);
	write("NICK " + _nick);
	write("USER " + _user);

	_running = true;
	// 2) Boucle de lecture
	char tmp[1024];
	while (_running)
	{
		ssize_t n = recv(_sockfd, tmp, sizeof(tmp), 0);
		if (n <= 0)
		{
			std::cout << "Disconnected from server\n";
			break;
		}
		// on reconstitue dans _buffer
		_buffer.append(tmp, n);

		// on cherche toutes les lignes completes
		std::vector<std::string> lines = split(_buffer, "\r\n");
		// dernier element = fragment résiduel
		_buffer = lines.back();

		// chaque ligne complete to handle
		for (size_t i = 0; i + 1 < lines.size(); ++i)
			handle_line(lines[i]);
	}

	close(_sockfd);
}



std::vector<std::string> TimeBot::split(const std::string &s, const std::string &delim)
{
	std::vector<std::string> out;
	size_t start = 0, pos;
	while ((pos = s.find(delim, start)) != std::string::npos)
	{
		out.push_back(s.substr(start, pos - start));
		start = pos + delim.length();
	}
	out.push_back(s.substr(start));
	return out;
	}

void    TimeBot::write(const std::string& str) 
{
	std::string message = str + "\r\n";
	ssize_t bytes_sent = send(_sockfd, message.c_str(), message.length(), 0);
	if (bytes_sent == -1)
		std::cerr << "Error while sending data" << std::endl;
}

std::vector<std::string> TimeBot::split_white_spaces(const std::string &str)
{
	std::vector<std::string> tokens;
	size_t i = 0;

	// 1) Si : present, skip it
	if (i < str.size() && str[i] == ':')
	{
		while (i < str.size() && str[i] != ' ') ++i;
		++i;
	}

	// 2) extraire jusqu'a trois tokens classiques
	for (int count = 0; count < 2 && i < str.size(); ++count)
	{
		// sskip spaces
		while (i < str.size() && std::isspace((unsigned char)str[i])) ++i;
		size_t start = i;
		while (i < str.size() && !std::isspace((unsigned char)str[i])) ++i;
		tokens.push_back(str.substr(start, i - start));
	}

	// 3) le reste : trailing parameter
	while (i < str.size() && std::isspace((unsigned char)str[i])) ++i;
	if (i < str.size())
	{
		tokens.push_back(str.substr(i));
	}

	return tokens;
}

void TimeBot::handle_line(const std::string &line)
{
	std::cout << "<<< " << line << "\n";

	// 1) Si la ligne commence par ":", on vire le prefixe ":nick!ident@host "
	std::string rest = line;
	if (!rest.empty() && rest[0] == ':')
	{
		size_t sp = rest.find(' ');
		if (sp == std::string::npos)
			return;
		rest = rest.substr(sp + 1);
	}

	// 2) Tokenisation en 3 tokens max + trailing
	std::vector<std::string> args = split_white_spaces(rest);
	if (args.empty()) return;

	// 4) PRIVMSG DM-only → !time
	if (args[0] == "PRIVMSG" && args.size() >= 3 && args[1] == _nick)
	{
		// args[2] = ":!time"
		std::string cmd = args[2];
		if (!cmd.empty() && cmd[0] == ':')
			cmd = cmd.substr(1);

		if (cmd == "time" || cmd == "!time")
		{
			// calcul de l'heure locale
			time_t now = time(NULL);
			char tbuf[9];
			strftime(tbuf, sizeof(tbuf), "%H:%M:%S", localtime(&now));

			// extraire l'expediteur du prefixe original
			// (on pouvait le stocker avant, ou le reprendre ici)
			size_t excl = line.find('!');
			std::string sender;
			if (line.size() > 1 && line[0] == ':' && excl != std::string::npos)
				sender = line.substr(1, excl - 1);
			write("PRIVMSG " + sender + " :It's " + tbuf);
		}
		else
		{
			size_t excl = line.find('!');
			std::string sender;
			if (line.size() > 1 && line[0] == ':' && excl != std::string::npos)
				sender = line.substr(1, excl - 1);
			write("NOTICE " + sender + " :Unknown command \"" + cmd + "\". Please use !time to get the current time.");
		}
	}
}
