# include "server/Server.hpp"

/*
    Corriger send() dans JOIN, qui ne fonctionne pas
    peut etre erreur de socket
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