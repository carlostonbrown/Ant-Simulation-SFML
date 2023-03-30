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
	diffusePheromones(0.01);

	for (float& cell : m_homePheromone)
	{
		if (cell > 0)
		{
			cell -= decay * dt.asSeconds();
		}
		
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

			HP -=500;

			HP /= 2;

			m_homePheromoneColour.a = HP;
			if (HP > 0)
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

	if (gridpos(x / m_cellSize, y / m_cellSize) <= m_homePheromone.size())
	{
		m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] += amount;

		if (m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] > 1000)
		{
			m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] = 1000;
		}

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

void Grid::diffusePheromones(float rate)
{

	std::vector<float> newHomePheromomne = m_homePheromone;

	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{

			float totalLevel = m_homePheromone[gridpos(x, y)];
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx == 0 && dy == 0) continue;
					int nx = x + dx;
					int ny = y + dy;
					if (nx < 0 || nx >= m_width || ny < 0 || ny >= m_height) continue;
					totalLevel += m_homePheromone[gridpos(nx,ny)];
				}
			}

			newHomePheromomne[gridpos(x, y)] = (1 - rate) * newHomePheromomne[gridpos(x, y)] + rate * (totalLevel / 9.0f);


		}

	}

	m_homePheromone = newHomePheromomne;



}

int Grid::windowToGridPos(int x, int y)
{
	return (x / m_cellSize) + (y / m_cellSize) * m_width;
}
