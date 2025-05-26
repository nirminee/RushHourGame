#ifndef GRILLE_HPP
#define GRILLE_HPP

// Inclusion des dépendances nécessaires
#include "Vehicule.hpp"
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>

// Classe représentant la grille du jeu Rush Hour
class Grille {
private:
    std::vector<std::vector<char>> grille;     // Représente visuellement la grille (6x6 par défaut) avec les identifiants des véhicules
    std::vector<Vehicule> vehicules;           // Liste des véhicules présents sur la grille
    std::map<char, sf::Color> colorMap;        // Associe chaque identifiant de véhicule à une couleur SFML
    int sortieX, sortieY;                      // Coordonnées de la sortie
    char sortieOrientation;                    // Orientation de la sortie ('r' pour droite)
    int largeur;                               // Largeur de la grille
    int hauteur;                               // Hauteur de la grille
    std::vector<std::vector<char>> cases;      // Redondant avec 'grille', peut servir à d’autres manipulations (ex : template de grille)

    // Met à jour la grille interne avec les positions actuelles des véhicules
    void mettreAJourGrille();

public:
    // Constructeur qui initialise la grille par défaut à 6x6 et vide
    Grille();

    // Charge un plateau depuis un fichier JSON et initialise les véhicules
    bool chargerPlateauDepuisJson(const std::string& chemin);

    // Tente de déplacer un véhicule selon une direction ("gauche", "droite", "haut", "bas")
    bool deplacerVehicule(char id, const std::string& direction);

    // Vérifie si la condition de victoire est atteinte (voiture 'X' est à la sortie)
    bool gagne() const;

    // Accès à la liste des véhicules (lecture seule)
    const std::vector<Vehicule>& getVehicules() const;

    // Accès à la carte des couleurs des véhicules
    const std::map<char, sf::Color>& getColorMap() const;

    // Retourne une chaîne de hachage représentant l’état actuel de la grille (utile pour éviter les répétitions dans les IA)
    std::string getHash() const;

    // Retourne le contenu d'une case de la grille (utilisé pour affichage ou vérification)
    char getCase(int x, int y) const;

    // Getters pour la taille de la grille et la position de la sortie
    int getLargeur() const { return largeur; }
    int getHauteur() const { return hauteur; }
    int getSortieX() const { return sortieX; }
    int getSortieY() const { return sortieY; }
};

#endif
