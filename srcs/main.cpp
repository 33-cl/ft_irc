# include "server/Server.hpp"

/*

    A Faire

        - se renseigner sur les macros pour les erreurs
        - implementer Serv& Server::get_instance() pour recuperer
          une copie de la structure pour l'utiliser dans Command
          (surement constructeur de copie et surcharge =)
        - renseignement sur KICK
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