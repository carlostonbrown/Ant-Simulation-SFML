#include <SFML/Graphics.hpp>
#include "Grid.h"


class Ant
{
public:
    Ant(sf::Vector2f position, sf::Vector2f velocity, sf::Color color);

    void update(sf::Time deltaTime,Grid& grid, int width, int height);
    void draw(sf::RenderWindow& window);
    

private:
    void layPheramones(Grid& grid, sf::Time deltaTime);
    void wander();
    sf::CircleShape m_shape;
    sf::Vector2f m_position;
    sf::Vector2f m_prevposition;
    sf::Vector2f m_colonyPosition;
    sf::Vector2f m_velocity;
    float m_pheromoneAmount;
    float decay = 100;
    float m_speed;
    float m_turnSpeed = 0.15;
    float m_pheromoneAngle = 60;
    float angleRadians = m_pheromoneAngle / 57.2957795;
    float m_wanderProbability = 0.2f;
    bool m_hasFood;
};

