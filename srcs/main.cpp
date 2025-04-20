# include "server/Server.hpp"

/*

	Probleme, si on est dans un term qu'on register en operator puis ctrl c et qu'ensuite on relnce nc localhost 4242 dans le meme term, 
	si on register avec un autre compte on sera deja present dans le channel!!! et donc operator

    PASS
        OK

	NOTICE
		A FAIRE

    NICK
        OK

    USER
        - Mauvais message d'erreur quand le format est mauvais

    JOIN
        OK

    PRIVMSG
        OK

    QUIT
		- le message est optionnel, doit pouvoir quitter avec "QUIT"
        - le message doit commencer par ':'
        - tout le clean devrait se faire dans le destructeur du client pour au lieu de QUIT, utile pour interpreter les signaux comme QUIT

    KICK
        - Ajouter la verif de l'operateur

    PART
        OK

    TOPIC
        add une chaine de caract
    
    MODE
        - si je fais "MODE #general" doit renvoyer :<serveur> 324 <nick> <canal> <modes> <mode_params>
		-gerer les topics
		- rien de teste avec hexchat atm

    Makefile
        ne recompile pas si je touch un .hpp

    A faire en dernier
    
		- les messages d'erreur via les throw sont is bon? faut pas mettre le nom du server devant?
            -> Oui c'est bon car le serv appelle client.send_msg qui print le nom du serv

        - Enlever la fonction infos() dans la boucle
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