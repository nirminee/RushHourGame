#ifndef VEHICULE_HPP
#define VEHICULE_HPP

class Vehicule {
private:
    char id;
    int x, y;
    int longueur;
    bool horizontal;

public:
    Vehicule(char id, int x, int y, int longueur, char orientation);
    char getId() const;
    int getX() const;
    int getY() const;
    int getLongueur() const;
    bool isHorizontal() const;
    void setX(int newX);
    void setY(int newY);
};

#endif
