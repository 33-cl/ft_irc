# include "server/Server.hpp"

/*

	Infinite loop
		- Quand on quitte avec la croix via hexchat(clean le tableau de socket)
    PASS
        OK

    NICK
        Affichage du message welcome si j'utilise NICK pour changer de nickname

    USER
<<<<<<< HEAD
        - check des ":" avant le nom du user
        - Parser le username : pas commencer par un chiffre etc.
=======
		OK
>>>>>>> 3d5e5e926a4cc189aacd5a34341c224bf2dc956e

    JOIN
        - Parser le nom des channels
        - Si JOIN avec plusieurs argument et erreur sur l'un, les suivants ne sont pas executes
    
    PRIVMSG
        - Check si channel/client existe
        - Diviser par 2 fonctions si client ou channel

    QUIT
        OK

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