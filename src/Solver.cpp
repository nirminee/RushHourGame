#include "Solver.hpp"
#include <sstream>

// Méthode qui résout automatiquement le puzzle en utilisant une recherche en largeur (BFS)
// Retourne un vecteur de chaînes de caractères correspondant aux mouvements à effectuer pour gagner
std::vector<std::string> Solver::résoudre(Grille grilleInitiale) {
    std::queue<État> file; // File FIFO utilisée pour la recherche en largeur
    std::unordered_set<std::string> visités; // Ensemble des états déjà explorés (par leur hash)

    // On commence avec l'état initial (grille de départ + chemin vide)
    file.push({grilleInitiale, {}});

    // Tant qu'il y a des états à explorer
    while (!file.empty()) {
        État courant = file.front(); // On récupère l'état en tête de la file
        file.pop(); // On le retire de la file

        // Si la grille courante est gagnante (le véhicule rouge est à la sortie)
        if (courant.grille.gagne()) {
            return courant.chemin; // On retourne le chemin qui mène à la victoire
        }

        // Pour chaque véhicule présent dans la grille
        for (const auto& v : courant.grille.getVehicules()) {
            // On teste tous les déplacements possibles : gauche, droite, haut, bas
            for (const auto& dir : {"gauche", "droite", "haut", "bas"}) {
                Grille copie = courant.grille; // On fait une copie de la grille pour tester un mouvement

                // Si le déplacement du véhicule est valide (sans collision et dans les limites)
                if (copie.deplacerVehicule(v.getId(), dir)) {
                    std::string hash = copie.getHash(); // On récupère un identifiant unique de la grille

                    // Si cette configuration n'a jamais été rencontrée
                    if (visités.find(hash) == visités.end()) {
                        visités.insert(hash); // On la marque comme visitée

                        // On copie le chemin actuel et on y ajoute le nouveau mouvement
                        std::vector<std::string> nouveauChemin = courant.chemin;
                        nouveauChemin.push_back(std::string(1, v.getId()) + " " + dir);

                        // On ajoute le nouvel état à la file pour exploration future
                        file.push({copie, nouveauChemin});
                    }
                }
            }
        }
    }

    // Si on sort de la boucle, c'est qu'aucune solution n'a été trouvée
    return {};
}
