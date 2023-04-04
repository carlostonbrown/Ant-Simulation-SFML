#include "Ant.h"

Ant::Ant(sf::Vector2f position, sf::Vector2f velocity, sf::Color color) :
    m_shape(3.f),
    m_position(position),
    m_colonyPosition(position),
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


    //follow pheramones 

    if (m_hasFood)
    {
        // Follow home pheromones
        sf::Vector2f direction = grid.getHomePheromoneDirection(m_position.x, m_position.y);
        if (direction != sf::Vector2f(0, 0)) 
        {
            // Turn towards the direction of the pheromone
            sf::Vector2f newVelocity = m_velocity + direction * m_turnSpeed;
            float length = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
            m_velocity = newVelocity / length;
        }
        sf::Vector2f vectorToColony = m_position - m_colonyPosition;
        float distanceToColony = std::sqrt(vectorToColony.x * vectorToColony.x + vectorToColony.y * vectorToColony.y);
        if (distanceToColony < 10)
        {
            m_velocity *= -1.0f;
            m_hasFood = false;
            m_pheromoneAmount = 1000;
        }


    }
    else
    {
        //follow food pheramones
        sf::Vector2f direction = grid.getFoodPheromoneDirection(m_position.x, m_position.y);
        if (direction != sf::Vector2f(0, 0))
        {
            // Turn towards the direction of the pheromone
            sf::Vector2f newVelocity = m_velocity + direction * m_turnSpeed;
            float length = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
            m_velocity = newVelocity / length;
        }

        if (grid.hasfood(m_position.x, m_position.y))
        {
            m_velocity *= -1.0f;
            m_hasFood = true;
            m_pheromoneAmount = 100;
        }
    }


    wander();


   

    if (grid.isWalls(m_position.x, m_position.y))
    {

        // Reflect velocity off wall
        sf::Vector2f normal = grid.getCellNormal(m_position.x, m_position.y);
        m_velocity = m_velocity - 2.0f * normal * (normal.x * m_velocity.x + normal.y * m_velocity.y);

        while (grid.isWalls(m_position.x, m_position.y))
        {
            m_position += normal * deltaTime.asSeconds();
        }


    }

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



    m_position += m_velocity * deltaTime.asSeconds() * m_speed;
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
    if (m_hasFood)
    {
        // lay food pheromone 
        grid.addFoodPheromone(m_position.x, m_position.y, m_pheromoneAmount * deltaTime.asSeconds());
        m_pheromoneAmount -= decay * deltaTime.asSeconds();

    }
    else
    {
        grid.addHomePheromone(m_position.x, m_position.y, m_pheromoneAmount * deltaTime.asSeconds());
        m_pheromoneAmount -= decay * deltaTime.asSeconds();
    }
    

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



