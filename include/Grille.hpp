#ifndef GRILLE_HPP
#define GRILLE_HPP

#include "Vehicule.hpp"
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class Grille {
private:
    std::vector<std::vector<char>> grille;
    std::vector<Vehicule> vehicules;
    std::map<char, sf::Color> colorMap;
    int sortieX, sortieY;
    char sortieOrientation;
    int largeur;
    int hauteur;
    std::vector<std::vector<char>> cases; // Use a dynamic 2D vector

    void mettreAJourGrille();

public:
    Grille();
    bool chargerPlateauDepuisJson(const std::string& chemin);
    bool deplacerVehicule(char id, const std::string& direction);
    bool gagne() const;
    const std::vector<Vehicule>& getVehicules() const;
    const std::map<char, sf::Color>& getColorMap() const;
    std::string getHash() const;
    char getCase(int x, int y) const;
    int getLargeur() const { return largeur; }
    int getHauteur() const { return hauteur; }
    int getSortieX() const { return sortieX; }
    int getSortieY() const { return sortieY; }
};

#endif
