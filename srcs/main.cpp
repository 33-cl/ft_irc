# include "server/Server.hpp"

/*

	CHANNEL
		- ajouter une str pour le topic
		- ajouter un map de booleens pour les modes
		- une liste de clients pour les invites
		- une liste d'operateurs
	Infinite loop
		- Quand on quitte avec la croix via hexchat(clean le tableau de socket)
    PASS
        OK

    NICK
        OK

    USER
		OK

    JOIN
        OK 
    
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
        add une chaine de caract
    
    MODE
        A FAIRE
		- map de booleens, is t 1 or 0

    Makefile
        ne recompile pas si je touch un .hpp

    A faire en dernier

        - virer les commentaires inutiles
        - traduire les commentaires utiles en anglais
        - remplacer les wildcards
        - supprimer ADMIN
		- changer le server name dans la liste d'init de server(setup.cpp)
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