#include "AntColony.h"

//constructor for the Ant colony class
AntColony::AntColony(sf::Vector2f position, int antCount, sf::Color color) :
    m_shape(20.f),
    m_position(position),
    m_color(color)

{
    // Set the properties of the colony's shape for rendering
    m_shape.setFillColor(m_color);
    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
    m_shape.setPosition(m_position);

    // Spawn the initial ants for the colony
    for (int i = 0; i < antCount; i++)
    {
        spawnAnt();
    }
}


// Function to spawn a new ant for the colony
void AntColony::spawnAnt()
{
    // Generate a random velocity for the ant
    sf::Vector2f velocity;
    float angle = (rand() %1000000);

    velocity.x = cos(angle);
    velocity.y = sin(angle);

    // Create a new ant and add it to the colony's list of ants
    Ant newAnt(m_position, velocity, m_color);
    m_ants.push_back(newAnt);


}
void AntColony::setPosition(sf::Vector2f position)
{
    m_position = position;
}
sf::Vector2f AntColony::getPosition()
{

    return m_position;
}
// Update the ant colony and its ants
void AntColony::update(sf::Time deltaTime, Grid& grid, int width, int height)
{

    for (Ant &ant : m_ants)
    {
        ant.update(deltaTime,grid,width,height);
        // If the ant is carrying food and is close to the colony, increment the food counter
        if (ant.getHasFood())
        {
            sf::Vector2f vectorToAnt = m_position - ant.getPosition();
            float distanceToAnt = std::sqrt(vectorToAnt.x * vectorToAnt.x + vectorToAnt.y * vectorToAnt.y);

            if (distanceToAnt <= 30)
            {
                if (m_ants.size() < antCountMax)
                {
                    food += 1;
                }
                


            }


        }
    }
    // If the colony has enough food, spawn a new ant
    if (food > 5)
    {
        spawnAnt();
        food = 0;
    }
    
    // Add home pheromones around the colony
    for (int i = -2; i < 2; i++)
    {
        for (int j = -2; j < 2; j++)
        {
            grid.addHomePheromone(m_position.x+ i*5, m_position.y+j * 5, 10000);
        }
    }


   

}
// Draw the ant colony and its ants
void AntColony::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    for (Ant &ant : m_ants)
    {
        ant.draw(window);
    }

}