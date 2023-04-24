#include "Ant.h"

Ant::Ant(sf::Vector2f position, sf::Vector2f velocity, sf::Color color) :
    m_shape(3.f),
    m_position(position),
    m_prevposition(position),
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
    float randomChance = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (randomChance < m_wanderProbability)
    {
        wander();

    }
    else
    {



        //follow pheramones 

        if (m_hasFood)
        {
            // Follow home pheromones
            //sf::Vector2f direction = grid.getHomePheromoneDirection(m_position.x, m_position.y);
            sf::Vector2f direction = grid.getHomePheromoneDirectionFO(m_position.x, m_position.y, m_velocity, angleRadians);

            if (direction != sf::Vector2f(0, 0))
            {
                // Turn towards the direction of the pheromone
                sf::Vector2f newVelocity = m_velocity + direction * m_turnSpeed;
                float length = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
                m_velocity = newVelocity / length;
            }
           


        }
        else
        {
            //follow food pheramones
            //sf::Vector2f direction = grid.getFoodPheromoneDirection(m_position.x, m_position.y);
            sf::Vector2f direction = grid.getFoodPheromoneDirectionFO(m_position.x, m_position.y, m_velocity, angleRadians);
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
                m_pheromoneAmount = 2000;
                grid.removeFood(m_position.x, m_position.y, 1);
            }
        }

    }

    if (m_pheromoneAmount < 1950)
    {
        sf::Vector2f vectorToColony = m_position - m_colonyPosition;
        float distanceToColony = std::sqrt(vectorToColony.x * vectorToColony.x + vectorToColony.y * vectorToColony.y);
        if (distanceToColony < 20)
        {
            
            m_hasFood = false;
            m_pheromoneAmount = 2000;

            m_position = m_colonyPosition;
            sf::Vector2f randomDirection(rand() % 201 - 100, rand() % 201 - 100);
            float length = std::sqrt(randomDirection.x * randomDirection.x + randomDirection.y * randomDirection.y);
            randomDirection /= length;
            m_velocity = randomDirection;

        }
    }
    
    wander();

    float movementStep = m_speed * deltaTime.asSeconds();
    int steps = static_cast<int>(std::ceil(movementStep / grid.getCellSize()));

    // Calculate the distance the ant will travel in each step
    sf::Vector2f stepDirection = m_velocity * deltaTime.asSeconds() / (float)steps;

    // Move the ant in smaller steps and check for collisions at each step
    for (int i = 0; i < steps; i++)
    {
        sf::Vector2f newPosition = m_position + stepDirection;
        if (!grid.isWalls(newPosition.x, newPosition.y))
        {
            m_position = newPosition;
        }
        else
        {
            
            m_velocity = -m_velocity;
            newPosition = m_position + stepDirection * (float)(steps - i - 1); 
            if (!grid.isWalls(newPosition.x, newPosition.y))
            {
                m_position = newPosition;
            }
            break; // Break the loop to stop moving after a collision
        }
    }




    

    

    // If the ant is still within a wall, move it to the nearest empty cell
    if (grid.isWalls(m_position.x, m_position.y))
    {
       
        m_position = m_prevposition;
    }

    m_prevposition = m_position;

    m_position += m_velocity * deltaTime.asSeconds() * m_speed;
    m_shape.setPosition(m_position);
   

    if (m_pheromoneAmount > 0)
    {
        layPheramones(grid, deltaTime);
    }
   
    
    if (m_position.x <= 0)
    {
        m_position.x = 0;
        m_velocity.x = -m_velocity.x;
    }
    else if (m_position.x >= width - 0)
    {
        m_position.x = width - 0;
        m_velocity.x = -m_velocity.x;
    }
    if (m_position.y <= 0)
    {
        m_position.y = 0;
        m_velocity.y = -m_velocity.y;
    }
    else if (m_position.y >= height - 10)
    {
        m_position.y = height - 10;
        m_velocity.y = -m_velocity.y;
    }
    
    
}

void Ant::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);



    if (m_hasFood)
    {
        sf::Color color = m_shape.getFillColor();
        float radius = m_shape.getRadius();
        m_shape.setFillColor(sf::Color::Green);
        m_shape.setRadius(2);
        m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
        m_shape.setPosition(m_position);
        window.draw(m_shape);
        m_shape.setFillColor(color);
        m_shape.setRadius(radius);
        m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
        m_shape.setPosition(m_position);
    }
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



