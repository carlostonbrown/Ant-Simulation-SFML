#pragma once
#include <SFML/Graphics.hpp>
#include "AntColony.h"
#include "Grid.h"
class Game
{
public:
    Game();
    void Run();

private:
    int width = 1500;
    int height = 900;
    int cellsize = 10;
    float m_simSpeed = 1;
    bool running = false;
    sf::RenderWindow window;
    sf::CircleShape m_cursorShape;
    static const sf::Time TimePerFrame;

    AntColony m_colony;
    Grid m_grid;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);





};

