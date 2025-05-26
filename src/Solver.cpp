#include "Solver.hpp"
#include <sstream>

std::vector<std::string> Solver::résoudre(Grille grilleInitiale) {
    std::queue<État> file;
    std::unordered_set<std::string> visités;

    file.push({grilleInitiale, {}});

    while (!file.empty()) {
        État courant = file.front();
        file.pop();

        if (courant.grille.gagne()) {
            return courant.chemin;
        }

        for (const auto& v : courant.grille.getVehicules()) {
            for (const auto& dir : {"gauche", "droite", "haut", "bas"}) {
                Grille copie = courant.grille;
                if (copie.deplacerVehicule(v.getId(), dir)) {
                    std::string hash = copie.getHash();
                    if (visités.find(hash) == visités.end()) {
                        visités.insert(hash);
                        std::vector<std::string> nouveauChemin = courant.chemin;
                        nouveauChemin.push_back(std::string(1, v.getId()) + " " + dir);
                        file.push({copie, nouveauChemin});
                    }
                }
            }
        }
    }

    return {};
}
