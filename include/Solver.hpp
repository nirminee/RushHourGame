
#pragma once
#include "Grille.hpp"
#include <queue>
#include <unordered_set>
#include <string>

class Solver {
public:
    struct État {
        Grille grille;
        std::vector<std::string> chemin;
    };

    static std::vector<std::string> résoudre(Grille grilleInitiale);
};
