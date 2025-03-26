# include "server/Server.hpp"

/*
    A faire en dernier

        - virer les commentaires inutiles
        - traduire les commentaires utiles en anglais
        - remplacer les wildcards
        - supprimer ADMIN
*/

/*
➜  ft_irc git:(main) ✗ nc localhost 4141
    ADMIN
    JOIN #general dfasdf
    :admin!admin@localhost JOIN #general
    :ircserv 476 dfasdf :Bad channel mask

    JOIN #general
    :ircserv 443 admin #general :is already on channel

*/

int main(int argc, char **argv)
{
    Server  serv;

    try
    {
        serv.initialize(argc, argv);
        serv.start();
    }
    catch (const std::exception& e) {std::cerr << e.what() << '\n'; return 1;}
} 