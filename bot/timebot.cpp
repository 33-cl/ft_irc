#include "timebot.hpp"

// Paramètres de connexion
static const char* SERVER_IP   = "127.0.0.1";
static const int   SERVER_PORT = 4242;
static const char* PASSWORD    = "password";
static const char* BOT_NICK    = "TimeBot";
static const char* BOT_USER    = "TimeBot 0 * :Time Bot";

int main()
{
    // 1) Création et connexion du socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
	{
		perror("socket");
		return 1;
	}
    struct sockaddr_in serv;

    std::memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port   = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv.sin_addr) <= 0)
	{
        std::cerr << "Adresse serveur invalide\n";
        return 1;
    }
    if (connect(sock, (struct sockaddr*)&serv, sizeof(serv)) < 0)
	{
        perror("connect"); return 1;
    }

    // 2) Init Client pour utiliser write()
    Client bot(sock);
    bot.write(std::string("PASS ") + PASSWORD);
    bot.write(std::string("NICK ") + BOT_NICK);
    bot.write(std::string("USER ") + BOT_USER);

    // 3) Boucle de réception-simple
    char buffer[1024];
    while (true)
	{
        ssize_t bytes_received = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (bytes_received <= 0)
			break;
        buffer[bytes_received] = '\0';
        std::string buffer_str(buffer);
        if (buffer_str.size() < 4)
			continue;// ne rien faire si on a pas recu au minimum 4 octet
        
        std::vector<std::string> commands = split(buffer_str, "\r\n");
        for (size_t i = 0; i < commands.size(); ++i)
		{
            std::string &cmd = commands[i];
            std::cout << "cmd: " << cmd << std::endl;
            std::vector<std::string> args = split_white_spaces(cmd);
            if (args.empty())
				continue;

            // Gérer PING, opermet de verifier que le client est tjrs actif et que le server ne ferme pas la connexion
            if (args[0] == "PING" && args.size() >= 2)
			{
                bot.write(std::string("PONG ") + args[1]);
                continue;
            }

            // DM-only: PRIVMSG TimeBot :!time
            if (args[0] == "PRIVMSG" && args.size() >= 3 && args[1] == BOT_NICK)
			{
                size_t pos = cmd.find(" :");
                if (pos == std::string::npos)
					continue;
                std::string message = cmd.substr(pos + 2);
                if (message == "!time")
				{
                    // Heure locale
                    time_t now = time(NULL);
                    char timestr[9];
                    strftime(timestr, sizeof(timestr), "%H:%M:%S", localtime(&now));
                    
                    // Expéditeur, pouvoir ecrire a la personne qui a fait la demande au bot
                    std::string prefix = args[0];
                    std::string prefixToken = args[0]; // en vrai, reprendre le token original
                    size_t excl = prefixToken.find('!');
                    std::string sender = (excl != std::string::npos)
                        ? prefixToken.substr(1, excl - 1)
                        : prefixToken.substr(1);
                    
                    bot.write("PRIVMSG " + sender + " :Il est " + timestr);
                }
            }
        }
    }
    return 0;
}
