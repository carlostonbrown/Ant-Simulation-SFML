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
    int width = 1920;
    int height = 1080;

    sf::RenderWindow window;

    static const sf::Time TimePerFrame;

    AntColony m_colony;
    Grid m_grid;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);





};

