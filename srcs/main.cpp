# include "server/Server.hpp"

/*
	Dans nick, doit commencer par une lettre et accepte -, _, [, ], {, }, \\, |, ^

    A Faire
        - se renseigner sur les macros pour les erreurs
        - renseignement sur KICK
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