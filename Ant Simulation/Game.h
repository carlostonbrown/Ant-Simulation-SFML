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
    int width = 1000;
    int height = 1000;
    float m_simSpeed = 1;
    sf::RenderWindow window;

    static const sf::Time TimePerFrame;

    AntColony m_colony;
    Grid m_grid;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);





};

