/*
    GESTION D'ERREURS

    1.Erreurs récupérables
        Exemples : Mot de passe incorrect, commande mal formatée, canal inexistant.
        Traitement : try -> catch, envoyer un message d'erreur via send(), permettre au client de réessayer.

    2.Erreurs critiques
        Exemples : Défaillance du serveur, erreur de mémoire, problème réseau.
        Traitement : try -> catch, journaliser l'erreur, fermer la connexion ou redémarrer, ne pas permettre de réessayer immédiatement.

    3.Erreurs de validation
        Exemples : Pseudonyme déjà utilisé, canal plein, tentative non autorisée.
        Traitement : Envoyer un message d'erreur via send(), bloquer l'action, permettre une nouvelle tentative avec des données valides.

    En bref :
    Récupérables : Client peut réessayer.
    Critiques : Problème grave, fermer ou redémarrer.
    Validation : Bloquer l'action, demander une correction.

*/

#pragma once

#define ERR_NONICKNAMEGIVEN(client) "431 " + client + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(nick) "432 " + nick + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(nick) "433 " + nick + " " + nick + " nickname is already in use"
#define ERR_NEEDMOREPARAMS(client, command) "461 " + client + " " + command + " :Not enough parameters"
#define ERR_PASSWDMISMATCH(client) "464 " + client + " :Password incorrect"
