#include "AntColony.h"

AntColony::AntColony(sf::Vector2f position, int antCount, sf::Color color) :
    m_shape(20.f),
    m_position(position),
    m_color(color)

{

    m_shape.setFillColor(m_color);
    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
    m_shape.setPosition(m_position);

    for (int i = 0; i < antCount; i++)
    {
        spawnAnt();
    }
}



void AntColony::spawnAnt()
{
    sf::Vector2f velocity;
    float angle = (rand() %1000000);

    velocity.x = cos(angle);
    velocity.y = sin(angle);
    Ant newAnt(m_position, velocity, m_color);
    m_ants.push_back(newAnt);


}
void AntColony::update(sf::Time deltaTime, Grid& grid, int width, int height)
{

    for (Ant &ant : m_ants)
    {
        ant.update(deltaTime,grid,width,height);
    }
    
    for (int i = -2; i < 2; i++)
    {
        for (int j = -2; j < 2; j++)
        {
            grid.addHomePheromone(m_position.x+ i*5, m_position.y+j * 5, 10000);
        }
    }


    grid.addHomePheromone(m_position.x, m_position.y, 10000);

}
void AntColony::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    for (Ant &ant : m_ants)
    {
        ant.draw(window);
    }

}