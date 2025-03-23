# include "server/Server.hpp"

/*

    A Faire

        - renseignement sur KICK -> nouvelle classe ds cmd
        - remplacer toutes les utilisations d'accesseur par
          l'utilisation directe de la variable avec les classes "friends"
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