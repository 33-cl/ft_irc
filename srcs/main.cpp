# include "server/Server.hpp"

/*
    A Faire

    A faire en dernier

        - virer les commentaires inutiles
        - traduire les commentaires utiles en anglais
        - remplacer les wildcards
        - supprimer ADMIN
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