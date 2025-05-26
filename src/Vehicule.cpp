#include "Vehicule.hpp"

Vehicule::Vehicule(char id, int x, int y, int longueur, char orientation)
    : id(id), x(x), y(y), longueur(longueur), horizontal(orientation == 'h') {}

char Vehicule::getId() const { return id; }
int Vehicule::getX() const { return x; }
int Vehicule::getY() const { return y; }
int Vehicule::getLongueur() const { return longueur; }
bool Vehicule::isHorizontal() const { return horizontal; }
void Vehicule::setX(int newX) { x = newX; }
void Vehicule::setY(int newY) { y = newY; }
