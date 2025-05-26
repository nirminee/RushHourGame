#include "Grille.hpp"       // Inclusion de la définition de la classe Grille
#include <fstream>          // Pour lire un fichier (plateau en JSON)
#include <sstream>          // Pour créer des chaînes de caractères formatées (utile pour getHash)
#include "json.hpp"         // On utilise la bibliothèque nlohmann/json pour lire les fichiers .json

using json = nlohmann::json; // On simplifie l'utilisation de la bibliothèque JSON

// Constructeur par défaut : initialise une grille 6x6 vide (avec des points '.')
Grille::Grille() : largeur(6), hauteur(6), cases(6, std::vector<char>(6, '.')) {}

// Fonction qui charge un plateau depuis un fichier JSON
bool Grille::chargerPlateauDepuisJson(const std::string& chemin) {
    std::ifstream fichier(chemin); // Ouvre le fichier
    if (!fichier) return false;    // Si le fichier n’existe pas ou n’est pas lisible, on arrête

    json donnees;
    fichier >> donnees; // Lecture du fichier dans un objet JSON

    vehicules.clear();  // On vide les anciens véhicules
    colorMap.clear();   // Et les anciennes couleurs

    // On lit la largeur et la hauteur depuis le JSON
    largeur = donnees["largeur"].get<int>();
    hauteur = donnees["hauteur"].get<int>();

    // On initialise la grille vide avec la bonne taille
    cases.assign(hauteur, std::vector<char>(largeur, '.'));

    // --- Chargement de la voiture rouge (départ, identifiant 'X') ---
    auto& vd = donnees["voiture_dep"];
    vehicules.emplace_back(
        vd["id"].get<std::string>()[0],
        vd["x"].get<int>() - 1,
        vd["y"].get<int>() - 1,
        vd["taille"].get<int>(),
        (vd["orientation"].get<std::string>() == "verticale") ? 'v' : 'h'
    );

    // On récupère la couleur RVB et on la stocke dans colorMap
    std::string couleur = vd["couleur"].get<std::string>();
    int r, g, b;
    sscanf(couleur.c_str(), "rvb(%d, %d, %d)", &r, &g, &b);
    colorMap[vd["id"].get<std::string>()[0]] = sf::Color(r, g, b);

    // --- Chargement des autres véhicules ---
    for (const auto& v : donnees["voitures"]) {
        vehicules.emplace_back(
            v["id"].get<std::string>()[0],
            v["x"].get<int>() - 1,
            v["y"].get<int>() - 1,
            v["taille"].get<int>(),
            (v["orientation"].get<std::string>() == "verticale") ? 'v' : 'h'
        );

        std::string c = v["couleur"].get<std::string>();
        sscanf(c.c_str(), "rvb(%d, %d, %d)", &r, &g, &b);
        colorMap[v["id"].get<std::string>()[0]] = sf::Color(r, g, b);
    }

    // --- Position de la sortie ---
    sortieX = donnees["sortie"]["x"].get<int>() - 1;
    sortieY = donnees["sortie"]["y"].get<int>() - 1;
    sortieOrientation = 'r'; // On suppose toujours que la sortie est à droite (right)

    mettreAJourGrille(); // Met à jour la grille en plaçant les véhicules
    return true;
}

// Fonction pour retourner le contenu d'une case donnée ('.' si hors limites)
char Grille::getCase(int x, int y) const {
    if (x < 0 || x >= largeur || y < 0 || y >= hauteur) return '.';
    return grille[y][x];
}

// Met à jour la grille en plaçant les véhicules à leur position
void Grille::mettreAJourGrille() {
    grille.assign(hauteur, std::vector<char>(largeur, '.')); // On vide la grille

    for (const auto& v : vehicules) {
        for (int i = 0; i < v.getLongueur(); ++i) {
            int xi = v.isHorizontal() ? v.getX() + i : v.getX(); // Position x de la case occupée
            int yi = v.isHorizontal() ? v.getY() : v.getY() + i; // Position y de la case occupée

            if (xi >= 0 && xi < largeur && yi >= 0 && yi < hauteur) {
                grille[yi][xi] = v.getId(); // On place l'identifiant du véhicule
            }
        }
    }
}

// Tente de déplacer un véhicule donné dans une direction ("gauche", "droite", "haut", "bas")
bool Grille::deplacerVehicule(char id, const std::string& direction) {
    for (auto& v : vehicules) {
        if (v.getId() != id) continue; // Si ce n’est pas le bon véhicule, on continue

        int dx = 0, dy = 0; // Déplacement horizontal ou vertical
        if (direction == "gauche" && v.isHorizontal()) dx = -1;
        else if (direction == "droite" && v.isHorizontal()) dx = 1;
        else if (direction == "haut" && !v.isHorizontal()) dy = -1;
        else if (direction == "bas" && !v.isHorizontal()) dy = 1;
        else return false; // Si direction invalide, on échoue

        // On vérifie que toutes les nouvelles cases sont valides (vides ou appartenant au même véhicule)
        for (int i = 0; i < v.getLongueur(); ++i) {
            int nx = v.getX() + (v.isHorizontal() ? i : 0) + dx;
            int ny = v.getY() + (v.isHorizontal() ? 0 : i) + dy;

            if (nx < 0 || nx >= largeur || ny < 0 || ny >= hauteur) return false;
            if (grille[ny][nx] != '.' && grille[ny][nx] != v.getId()) return false;
        }

        // Si déplacement possible, on change la position du véhicule
        v.setX(v.getX() + dx);
        v.setY(v.getY() + dy);
        mettreAJourGrille(); // Met à jour les positions sur la grille
        return true;
    }

    return false; // Si aucun véhicule trouvé, on retourne false
}

// Vérifie si la voiture 'X' est en position de gagner
bool Grille::gagne() const {
    for (const auto& v : vehicules) {
        if (v.getId() == 'X') {
            if (sortieOrientation != 'r') return false; // La sortie est à droite uniquement
            if (!v.isHorizontal()) return false;        // La voiture rouge doit être horizontale

            int finX = v.getX() + v.getLongueur() - 1; // Dernière case de la voiture X
            int finY = v.getY();

            // La grille va de 0 à 5 → la sortie est à droite (x = 5)
            return (finX == sortieX) && (finY == sortieY); // Si la voiture atteint la sortie
        }
    }

    return false;
}

// Renvoie la liste des véhicules (accès en lecture seule)
const std::vector<Vehicule>& Grille::getVehicules() const {
    return vehicules;
}

// Renvoie la map des couleurs pour chaque véhicule
const std::map<char, sf::Color>& Grille::getColorMap() const {
    return colorMap;
}

// Renvoie un identifiant unique (hash) de la position de tous les véhicules
std::string Grille::getHash() const {
    std::stringstream ss;
    for (const auto& v : vehicules) {
        ss << v.getId() << v.getX() << v.getY(); // Concatène l'ID + X + Y
    }
    return ss.str(); 
}
