#pragma once // Cette directive évite d'inclure ce fichier plusieurs fois pendant la compilation (protection contre les doubles inclusions).

#include "Grille.hpp"       // On a besoin de la classe Grille pour utiliser les grilles du jeu Rush Hour.
#include <queue>            // Pour utiliser la file (FIFO), essentielle à l'algorithme BFS.
#include <unordered_set>    // Pour stocker les états déjà visités, très rapide en recherche.
#include <string>           // Pour manipuler des chaînes de caractères.

class Solver {
public:

    // Structure représentant un état du jeu :
    // Elle contient une grille à un instant donné,
    // ainsi que le chemin (suite de mouvements) qui a mené à cette grille.
    struct État {
        Grille grille;                  // L'état actuel de la grille.
        std::vector<std::string> chemin; // Les déplacements effectués pour atteindre cet état.
    };

    // Méthode statique pour résoudre un puzzle de Rush Hour en partant d'une grille initiale.
    // Elle retourne un vecteur de chaînes représentant les mouvements à effectuer.
    static std::vector<std::string> résoudre(Grille grilleInitiale);
};
