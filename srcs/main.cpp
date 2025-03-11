# include "../includes/Server.hpp"

/*
    LOGIN
    - Implementer autres erreurs de login
    - Finir USER
    - Supprimer les getters/setters inutiles

    CHANNEL
    - Implementer la classe channel et les autres en consequences
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