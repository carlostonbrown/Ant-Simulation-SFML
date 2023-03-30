#include "Ant.h"

Ant::Ant(sf::Vector2f position, sf::Vector2f velocity, sf::Color color) :
    m_shape(3.f),
    m_position(position),
    m_velocity(velocity),
    m_hasFood(false),
    m_speed(100),
    m_pheromoneAmount(1000)
{
    m_shape.setFillColor(color);
    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
    m_shape.setPosition(m_position);

}



void Ant::update(sf::Time deltaTime,Grid& grid,int width, int height)
{

    wander();


    m_position += m_velocity * deltaTime.asSeconds() * m_speed;



    if (m_position.x <= 10)
    {
        m_position.x = 10;
        m_velocity.x = -m_velocity.x;
    }
    else if (m_position.x >= width-10)
    {
        m_position.x = width-10;
        m_velocity.x = -m_velocity.x;
    }
    if (m_position.y <= 10)
    {
        m_position.y = 10;
        m_velocity.y = -m_velocity.y;
    }
    else if (m_position.y >= height-10)
    {
        m_position.y = height-10;
        m_velocity.y = -m_velocity.y;
    }

    m_shape.setPosition(m_position);
   

    if (m_pheromoneAmount > 0)
    {

        
        layPheramones(grid, deltaTime);
    }

    
    
}

void Ant::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}



void Ant::layPheramones(Grid& grid,sf::Time deltaTime)
{

    grid.addHomePheromone(m_position.x, m_position.y, m_pheromoneAmount * deltaTime.asSeconds());
    m_pheromoneAmount-= decay*deltaTime.asSeconds();

}

void Ant::wander()
{

    sf::Vector2f randomDirection(rand() % 201 - 100, rand() % 201 - 100);
    float length = std::sqrt(randomDirection.x * randomDirection.x + randomDirection.y * randomDirection.y);
    randomDirection /=length;

    m_velocity += randomDirection * m_turnSpeed;

    length = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
    m_velocity /= length;
}
