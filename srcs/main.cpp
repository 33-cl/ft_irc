# include "server/Server.hpp"

/*

	Probleme, si on est dans un term qu'on register en operator puis ctrl c et qu'ensuite on relnce nc localhost 4242 dans le meme term, 
	si on register avec un autre compte on sera deja present dans le channel!!! et donc operator


	CHANNEL
		- ajouter une str pour le topic
		- ajouter un map de booleens pour les modes
		- une liste de clients pour les invites
		- une liste d'operateurs
	Infinite loop
		- Quand on quitte avec la croix via hexchat(clean le tableau de socket)
    PASS
        OK

	NOTICE
		A FAIRE

    NICK
        OK

    USER
        - Parser le username : pas commencer par un chiffre etc.

    JOIN
        - OK

    PRIVMSG
        - Check si channel/client existe
        - Diviser par 2 fonctions si client ou channel

    QUIT
		OK

    KICK
        - Ajouter la verif de l'operateur

    PART
        done?

    TOPIC
        add une chaine de caract
    
    MODE
		-gerer les topics
		- rien de teste avec hexchat atm

    Makefile
        ne recompile pas si je touch un .hpp

    A faire en dernier

		- les messages d'erreur via les throw sont is bon? faut pas mettre le nom du server devant?
        - virer les commentaires inutiles
        - traduire les commentaires utiles en anglais
        - remplacer les wildcards
        - supprimer ADMIN
		- changer le server name dans la liste d'init de server(setup.cpp)
		- verifier qu'a aucun moment on cree pas des channel sans join(j'avais fait de la merde dans topic a ce niveau, je regarderai si tout est ok partout)
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