#include <SFML/Graphics.hpp>
#include "Grid.h"


class Ant
{
public:
    Ant(sf::Vector2f position, sf::Vector2f velocity, sf::Color color);

    void update(sf::Time deltaTime,Grid& grid, int width, int height);
    void draw(sf::RenderWindow& window);
    
    void layPheramones(Grid& grid, sf::Time deltaTime);

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_pheromoneAmount;
    float decay =5;
    float m_speed;
    bool m_hasFood;
};

