#ifndef VEHICULE_HPP
#define VEHICULE_HPP

// Classe représentant un véhicule (voiture ou camion) sur la grille de Rush Hour
class Vehicule {
private:
    char id;           // Identifiant unique du véhicule (ex : 'A', 'B', 'X', etc.)
    int x, y;          // Coordonnées (x, y) du coin supérieur gauche du véhicule dans la grille
    int longueur;      // Longueur du véhicule (2 pour voiture, 3 pour camion)
    bool horizontal;   // true = véhicule horizontal, false = véhicule vertical

public:
    // Constructeur
    // orientation = 'h' (horizontal) ou 'v' (vertical)
    Vehicule(char id, int x, int y, int longueur, char orientation);

    // Accesseurs
    char getId() const;         // Renvoie l'identifiant
    int getX() const;           // Renvoie la position X
    int getY() const;           // Renvoie la position Y
    int getLongueur() const;    // Renvoie la longueur
    bool isHorizontal() const;  // Renvoie true si horizontal, false sinon

    // Mutateurs (modifie la position)
    void setX(int newX);        // Modifie la position X
    void setY(int newY);        // Modifie la position Y
};

#endif
