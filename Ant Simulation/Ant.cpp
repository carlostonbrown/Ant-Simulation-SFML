#include "Ant.h"

// Constructor for the Ant class
Ant::Ant(sf::Vector2f position, sf::Vector2f velocity, sf::Color color) :
    m_shape(3.f),
    m_position(position),
    m_prevposition(position),
    m_colonyPosition(position),
    m_velocity(velocity),
    m_hasFood(false),
    m_speed(100),
    m_pheromoneAmount(2000)
{
    m_shape.setFillColor(color);
    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
    m_shape.setPosition(m_position);
}

// Update function for the Ant class
void Ant::update(sf::Time deltaTime, Grid& grid, int width, int height)
{
    // Wander based on random chance
    float randomChance = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (randomChance < m_wanderProbability)
    {
        wander(deltaTime);
    }
    else
    {
        // Follow pheromones
        if (m_hasFood)
        {
            // Follow home pheromones
            sf::Vector2f direction = grid.getHomePheromoneDirectionFO(m_position.x, m_position.y, m_velocity, angleRadians);

            // Turn towards the direction of the pheromone
            if (direction != sf::Vector2f(0, 0))
            {
                sf::Vector2f newVelocity = m_velocity + direction * m_turnSpeed * deltaTime.asSeconds();
                float length = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
                m_velocity = newVelocity / length;
            }
        }
        else
        {
            // Follow food pheromones
            sf::Vector2f direction = grid.getFoodPheromoneDirectionFO(m_position.x, m_position.y, m_velocity, angleRadians);

            // Turn towards the direction of the pheromone
            if (direction != sf::Vector2f(0, 0))
            {
                sf::Vector2f newVelocity = m_velocity + direction * m_turnSpeed * deltaTime.asSeconds();
                float length = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
                m_velocity = newVelocity / length;
            }

            // If the ant finds food, change direction, set m_hasFood to true, reset m_pheromoneAmount, and remove food from the grid
            if (grid.hasfood(m_position.x, m_position.y))
            {
                m_velocity *= -1.0f;
                m_hasFood = true;
                m_pheromoneAmount = 2000;
                grid.removeFood(m_position.x, m_position.y, 1);
            }
        }
    }

    // If the ant is at the colony and has food, drop the food and reset position, velocity, and m_pheromoneAmount
    if (m_pheromoneAmount < 1980)
    {
        sf::Vector2f vectorToColony = m_position - m_colonyPosition;
        float distanceToColony = std::sqrt(vectorToColony.x * vectorToColony.x + vectorToColony.y * vectorToColony.y);
        if (distanceToColony < 30)
        {
            if (m_hasFood)
            {
                m_hasFood = false;
            }

            m_pheromoneAmount = 2000;
            m_position = m_colonyPosition;
            sf::Vector2f randomDirection(rand() % 201 - 100, rand() % 201 - 100);

            float length = std::sqrt(randomDirection.x * randomDirection.x + randomDirection.y * randomDirection.y);
            randomDirection /= length;
            m_velocity = randomDirection;
        }
    }

    // Call wander function to add randomness to ant's movement
    wander(deltaTime);

    // Calculate the movement step
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

    // If the ant is still within a wall, move it to the previous position
    if (grid.isWalls(m_position.x, m_position.y))
    {
        m_position = m_prevposition;
    }

    m_prevposition = m_position;

    // Update the ant's position based on its velocity and speed
    m_position += m_velocity * deltaTime.asSeconds() * m_speed;
    m_shape.setPosition(m_position);

    // Lay pheromones if the ant has enough pheromones left
    if (m_pheromoneAmount > 0)
    {
        layPheramones(grid, deltaTime);
    }

    // Keep the ant within the window bounds
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

// Draw function for the Ant class
void Ant::draw(sf::RenderWindow& window)
{
    sf::Color color = m_shape.getFillColor();

    // If the ant has food, draw a green circle, if not, draw the ant
    if (m_hasFood)
    {
        
       
        m_shape.setFillColor(sf::Color::Green);
        window.draw(m_shape);
        m_shape.setFillColor(color);
       
    }
    else
    {
        m_shape.setFillColor(color);
        window.draw(m_shape);
    }

}

// Get the ant's position
sf::Vector2f Ant::getPosition()
{
    return m_position;
}
// Get whether the ant has food or not
bool Ant::getHasFood()
{
    return m_hasFood;
}


// Lay pheromones on the grid based on the ant's position and whether it has food or not
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
        // Lay home pheromone
        grid.addHomePheromone(m_position.x, m_position.y, m_pheromoneAmount * deltaTime.asSeconds());
        m_pheromoneAmount -= decay * deltaTime.asSeconds();
    }
    

}
// Wander function to add randomness to the ant's movement
void Ant::wander(sf::Time deltaTime)
{
    // Generate a random direction
    sf::Vector2f randomDirection(rand() % 201 - 100, rand() % 201 - 100);
    float length = std::sqrt(randomDirection.x * randomDirection.x + randomDirection.y * randomDirection.y);
    randomDirection /=length;

    // Update the ant's velocity based on the random direction
    m_velocity += randomDirection * m_turnSpeed * deltaTime.asSeconds();

    // Normalize the velocity
    length = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
    m_velocity /= length;
}



