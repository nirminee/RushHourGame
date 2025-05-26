#include <SFML/Graphics.hpp>
#include "Grille.hpp"
#include "Solver.hpp"
#include <iostream>
#include <sstream>
#include <filesystem> 

const int cellSize = 100;
char vehiculeActif = 'X';

// Fonction pour obtenir la couleur SFML à partir du colorMap
sf::Color getColorForId(char id, const std::map<char, sf::Color>& colorMap) {
    auto it = colorMap.find(id);
    return (it != colorMap.end()) ? it->second : sf::Color(100, 100, 255);
}

void launchManualMode(Grille& g, sf::Texture& carTexture) {
    sf::RenderWindow window(sf::VideoMode(g.getLargeur() * cellSize, g.getHauteur() * cellSize), "Rush Hour - Manuel");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("../assets/OpenSans-Regular.ttf")) {
        std::cerr << "Erreur lors du chargement de la police !" << std::endl;
    }

    bool victoire = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && !victoire) {
                if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
                    vehiculeActif = static_cast<char>('A' + (event.key.code - sf::Keyboard::A));
                }
                if (event.key.code == sf::Keyboard::X) vehiculeActif = 'X';

                if (event.key.code == sf::Keyboard::Left) g.deplacerVehicule(vehiculeActif, "gauche");
                if (event.key.code == sf::Keyboard::Right) g.deplacerVehicule(vehiculeActif, "droite");
                if (event.key.code == sf::Keyboard::Up) g.deplacerVehicule(vehiculeActif, "haut");
                if (event.key.code == sf::Keyboard::Down) g.deplacerVehicule(vehiculeActif, "bas");
            }
        }

        window.clear(sf::Color::White);

        // Dessiner la grille
        for (int y = 0; y < g.getHauteur(); ++y) {
            for (int x = 0; x < g.getLargeur(); ++x) {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 2, cellSize - 2));
                cell.setPosition(x * cellSize, y * cellSize);
                // Check if this cell is the sortie
                if (x == g.getSortieX() && y == g.getSortieY()) {
                    cell.setFillColor(sf::Color(230, 230, 230)); // Keep the default fill color
                    cell.setOutlineThickness(3);                // Make the border thicker
                    cell.setOutlineColor(sf::Color::Red);       // Set the border color to red
                } else {
                    cell.setFillColor(sf::Color(230, 230, 230));
                    cell.setOutlineThickness(1);
                    cell.setOutlineColor(sf::Color::Black);
                }
                window.draw(cell);
            }
        }

        // Dessiner les véhicules avec des sprites
        const auto& colorMap = g.getColorMap();
        for (const auto& v : g.getVehicules()) {
            sf::Sprite carSprite;
            carSprite.setTexture(carTexture);

            float scaleX, scaleY;
            if (v.isHorizontal()) {
                scaleX = v.getLongueur() * (float)cellSize / carTexture.getSize().x;
                scaleY = (float)cellSize / carTexture.getSize().y;
                carSprite.setScale(scaleX, scaleY);
                carSprite.setPosition(v.getX() * cellSize, v.getY() * cellSize);
            } else {
                // For vertical: stretch horizontally, then rotate
                scaleX = v.getLongueur() * (float)cellSize / carTexture.getSize().x;
                scaleY = (float)cellSize / carTexture.getSize().y;
                carSprite.setScale(scaleX, scaleY);
                carSprite.setRotation(90.f);
                carSprite.setOrigin(0, 0);
                // After rotation, position is (y, gridSize - x - 1)
                carSprite.setPosition((v.getX() + 1) * cellSize, v.getY() * cellSize);
            }

            carSprite.setColor(getColorForId(v.getId(), colorMap));
            window.draw(carSprite);
        }

        // Afficher le message de victoire si la voiture rouge est sortie
        if (g.gagne()) {
            victoire = true;
            sf::Text message;
            message.setFont(font);
            message.setString("🚗 La voiture rouge est sortie !");
            message.setCharacterSize(32);
            message.setFillColor(sf::Color::Red);
            message.setStyle(sf::Text::Bold);
            message.setPosition(30, 250);
            window.draw(message);
        }

        window.display();
    }
}

void animateSolution(Grille& g, const std::vector<std::string>& solution, sf::Texture& carTexture) {
    sf::RenderWindow window(sf::VideoMode(g.getLargeur() * cellSize, g.getHauteur() * cellSize), "Rush Hour - Solution IA");
    window.setFramerateLimit(2);

    sf::Font font;
    if (!font.loadFromFile("../assets/OpenSans-Regular.ttf")) {
        std::cerr << "Erreur lors du chargement de la police !" << std::endl;
    }

    size_t étape = 0;
    Grille copieGrille = g;
    bool victoire = false;

    while (window.isOpen() && étape <= solution.size()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (étape < solution.size()) {
            std::istringstream iss(solution[étape]);
            std::string id, dir;
            iss >> id >> dir;
            copieGrille.deplacerVehicule(id[0], dir);
            étape++;
        }

        window.clear(sf::Color::White);

        // Dessiner la grille
        for (int y = 0; y < copieGrille.getHauteur(); ++y) {
            for (int x = 0; x < copieGrille.getLargeur(); ++x) {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 2, cellSize - 2));
                cell.setPosition(x * cellSize, y * cellSize);
                // Check if this cell is the sortie
                if (x == g.getSortieX() && y == g.getSortieY()) {
                    cell.setFillColor(sf::Color(230, 230, 230)); // Keep the default fill color
                    cell.setOutlineThickness(3);                // Make the border thicker
                    cell.setOutlineColor(sf::Color::Red);       // Set the border color to red
                } else {
                    cell.setFillColor(sf::Color(230, 230, 230));
                    cell.setOutlineThickness(1);
                    cell.setOutlineColor(sf::Color::Black);
                }
                window.draw(cell);
            }
        }

        // Dessiner les véhicules avec des sprites
        const auto& colorMap = copieGrille.getColorMap();
        for (const auto& v : copieGrille.getVehicules()) {
            sf::Sprite carSprite;
            carSprite.setTexture(carTexture);

            float scaleX, scaleY;
            if (v.isHorizontal()) {
                scaleX = v.getLongueur() * (float)cellSize / carTexture.getSize().x;
                scaleY = (float)cellSize / carTexture.getSize().y;
                carSprite.setScale(scaleX, scaleY);
                carSprite.setPosition(v.getX() * cellSize, v.getY() * cellSize);
            } else {
                // For vertical: stretch horizontally, then rotate
                scaleX = v.getLongueur() * (float)cellSize / carTexture.getSize().x;
                scaleY = (float)cellSize / carTexture.getSize().y;
                carSprite.setScale(scaleX, scaleY);
                carSprite.setRotation(90.f);
                carSprite.setOrigin(0, 0);
                // After rotation, position is (y, gridSize - x - 1)
                carSprite.setPosition((v.getX() + 1) * cellSize, v.getY() * cellSize);
            }

            carSprite.setColor(getColorForId(v.getId(), colorMap));
            window.draw(carSprite);
        }

        // Afficher le message de victoire si la voiture rouge est sortie
        if (copieGrille.gagne()) {
            victoire = true;
            sf::Text message;
            message.setFont(font);
            message.setString("La voiture rouge est sortie !");
            message.setCharacterSize(32);
            message.setFillColor(sf::Color::Red);
            message.setStyle(sf::Text::Bold);
            message.setPosition(30, 250);
            window.draw(message);
        }

        window.display();
        if (victoire) sf::sleep(sf::seconds(2));
    }
}

void solveWithAI(Grille& g, sf::Texture& carTexture) {
    std::cout << "Recherche de solution..." << std::endl;
    auto solution = Solver::résoudre(g);

    if (solution.empty()) {
        std::cout << "Aucune solution trouvée !" << std::endl;
        return;
    }

    std::cout << "\nSolution trouvée en " << solution.size() << " coups :\n";
    for (const auto& coup : solution) {
        std::cout << "- " << coup << std::endl;
    }

    std::cout << "\nAppuyez sur une touche pour voir l'animation..." << std::endl;
    std::cin.ignore();
    std::cin.get();

    animateSolution(g, solution, carTexture);
}

int main() {
    std::cout << "=== RUSH HOUR ===" << std::endl;
    std::cout << "1. Jouer manuellement\n2. Résolution automatique (IA)\nChoix : ";

    int choix;
    std::cin >> choix;

    // List plateau files
    std::vector<std::string> plateaux;
    std::string plateauxDir = "../plateaux";
    std::cout << "\nPlateaux disponibles :\n";
    int idx = 1;
    for (const auto& entry : std::filesystem::directory_iterator(plateauxDir)) {
        if (entry.path().extension() == ".json") {
            plateaux.push_back(entry.path().filename().string());
            std::cout << idx++ << ". " << plateaux.back() << '\n';
        }
    }
    if (plateaux.empty()) {
        std::cerr << "Aucun plateau trouvé dans " << plateauxDir << std::endl;
        return 1;
    }

    std::cout << "Choisissez le numéro du plateau : ";
    int plateauChoix = 0;
    std::cin >> plateauChoix;
    while (plateauChoix < 1 || plateauChoix > (int)plateaux.size()) {
        std::cout << "Numéro invalide. Choisissez à nouveau : ";
        std::cin >> plateauChoix;
    }
    std::string plateauPath = plateauxDir + "/" + plateaux[plateauChoix - 1];

    Grille g;
    if (!g.chargerPlateauDepuisJson(plateauPath)) {
        std::cerr << "Erreur lors du chargement.\n";
        return 1;
    }

    // Charge la texture de la voiture
    sf::Texture carTexture;
    if (!carTexture.loadFromFile("../assets/car.png")) {
        std::cerr << "Erreur lors du chargement du sprite voiture !" << std::endl;
        return 1;
    }

    std::cout << "Plateau chargé : " << plateauPath << std::endl;
    std::cout << "Dimensions du plateau : " << g.getLargeur() << "x" << g.getHauteur() << std::endl;

    if (choix == 1) {
        launchManualMode(g, carTexture);
    } else {
        solveWithAI(g, carTexture);
    }

    return 0;
}