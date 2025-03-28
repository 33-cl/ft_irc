# include "server/Server.hpp"

/*
    PASS
        OK

    NICK
        OK

    USER
        - check des ":" avant le nom du user

    JOIN
        OK 
    
    PRIVMSG
        - Check si channel/client existe
        - Diviser par 2 fonctions si client ou channel

    QUIT
        - Segfault quand le client est dans un channel

    KICK
        - Ajouter la verif de l'operateur

    PART
        A FAIRE

    TOPIC
        A FAIRE
    
    MODE
        A FAIRE

    Makefile
        ne recompile pas si je touch un .hpp

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