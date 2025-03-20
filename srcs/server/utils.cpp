#include "Server.hpp"

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens; // Vecteur pour stocker les sous-chaînes
    size_t start = 0; // Position de départ pour la recherche
    size_t end = str.find(delimiter); // Trouver la première occurrence du séparateur

    // Boucle pour diviser la chaîne
    while (end != std::string::npos) {
        // Extraire la sous-chaîne entre 'start' et 'end'
        tokens.push_back(str.substr(start, end - start));

        // Mettre à jour 'start' pour ignorer le séparateur
        start = end + delimiter.length();

        // Chercher le prochain séparateur
        end = str.find(delimiter, start);
    }

    // Ajouter la dernière sous-chaîne (après le dernier séparateur)
    if (start < str.length()) {
        tokens.push_back(str.substr(start));
    }

    return tokens; // Retourner le vecteur de sous-chaînes
}