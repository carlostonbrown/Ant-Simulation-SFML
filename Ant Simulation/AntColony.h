#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Ant.h"


class AntColony
{
public:
    AntColony(sf::Vector2f position, int antCount, sf::Color color);

    void update(sf::Time deltaTime,Grid& grid, int width, int height);
    void draw(sf::RenderWindow& window);

private:
    sf::Color m_color;
    sf::CircleShape m_shape;
    sf::Vector2f m_position;
    std::vector<Ant> m_ants;


    void spawnAnt();



};

