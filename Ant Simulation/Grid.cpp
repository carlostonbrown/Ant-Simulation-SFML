#include "Grid.h"

Grid::Grid(int width, int height, int cellSize, sf::Color homePheromoneColour, sf::Color foodPheromoneColour) :
	m_width(width / cellSize), 
	m_height(height / cellSize), 
	m_cellSize(cellSize),
	m_homePheromoneColour(homePheromoneColour),
	m_foodPheromoneColour(foodPheromoneColour)

{
	m_cellShape.setSize(sf::Vector2f(m_cellSize, m_cellSize));


	for (int i = 0; i < m_width * m_height; i++)
	{
		float newHPh = 0;
		m_homePheromone.push_back(newHPh);

		float newFPh = 0;
		m_foodPheromone.push_back(newFPh);

		float newFood = 0;
		m_foodAmount.push_back(newFood);

		float newWall = 0;
		m_walls.push_back(newWall);
	}



}

void Grid::update(sf::Time dt)
{

	for (float& cell : m_homePheromone)
	{
		cell -= decay *dt.asSeconds();
	}



}

void Grid::draw(sf::RenderWindow& window)
{

	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			float x = i * m_cellSize;
			float y = j * m_cellSize;

			//draw home pheromone

			float HP = m_homePheromone[gridpos(i,j)];
			m_homePheromoneColour.a = HP/4;
			if (HP > 1)
			{
				m_cellShape.setFillColor(m_homePheromoneColour);

				m_cellShape.setPosition(x, y);
				window.draw(m_cellShape);
			}
			
			
			//draw food pheromone



			//draw food
			float food = m_foodAmount[gridpos(i, j)];
			if (food > 0)
			{
				m_cellShape.setFillColor(sf::Color::Green);

				m_cellShape.setPosition(x, y);
				window.draw(m_cellShape);

			}


			//draw walls
			if (m_walls[gridpos(i, j)])
			{
				m_cellShape.setFillColor(sf::Color::White);

				m_cellShape.setPosition(x, y);
				window.draw(m_cellShape);

			}


		}
	}




}

void Grid::addHomePheromone(int x, int y, float amount)
{


	m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] += amount;

		if (m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] > 255)
		{
			m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] = 255;
		}
}

void Grid::addFood(int x, int y, float amount)
{
	m_foodAmount[gridpos(x / m_cellSize, y / m_cellSize)] += amount;
}

void Grid::addWalls(int x, int y)
{

	m_walls[gridpos(x / m_cellSize, y / m_cellSize)] = true;
}

int Grid::gridpos(int x, int y)
{
	return x + (y * m_width);

}

int Grid::windowToGridPos(int x, int y)
{
	return (x / m_cellSize) + (y / m_cellSize) * m_width;
}
