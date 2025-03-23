# include "server/Server.hpp"

/*
    A Faire

        - se renseigner sur les macros pour les erreurs
        - renseignement sur KICK
        - remplacer toutes les utilisations d'accesseur par
          l'utilisation directe de la variable avec les classes "friends"


    A faire en dernier

        - virer les commentaires inutiles
        - traduire les commentaires utiles en anglais
        - remplacer les wildcards
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