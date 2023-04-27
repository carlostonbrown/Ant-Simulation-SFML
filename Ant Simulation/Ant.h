#include <SFML/Graphics.hpp>
#include "Grid.h"



class Ant
{
public:
    Ant(sf::Vector2f position, sf::Vector2f velocity, sf::Color color);

    void update(sf::Time deltaTime,Grid& grid, int width, int height);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();
    bool getHasFood();
private:
    void layPheramones(Grid& grid, sf::Time deltaTime);
    void wander(sf::Time deltaTime);
    sf::CircleShape m_shape;
    sf::Vector2f m_position;
    sf::Vector2f m_prevposition;
    sf::Vector2f m_colonyPosition;
    sf::Vector2f m_velocity;
    float m_pheromoneAmount;
    float decay = 50;
    float m_speed;
    float m_turnSpeed = 10;
    float m_pheromoneAngle = 60.0f;
    float angleRadians = static_cast<float>(m_pheromoneAngle / 57.2957795f);
    float m_wanderProbability = 0.2f;
    bool m_hasFood;
};

