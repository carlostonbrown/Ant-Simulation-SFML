#include "Grid.h"
#include <fstream>
#include <iostream>

// Grid constructor
Grid::Grid(int width, int height, int cellSize, sf::Color homePheromoneColour, sf::Color foodPheromoneColour) :
	m_width(width / cellSize), 
	m_height(height / cellSize), 
	m_cellSize(cellSize),
	m_homePheromoneColour(homePheromoneColour),
	m_foodPheromoneColour(foodPheromoneColour),
	m_wallColors(m_width* m_height)

{
	// Set the size of the cell shape
	m_cellShape.setSize(sf::Vector2f(m_cellSize, m_cellSize));

	// Initialize grid cells
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
// Update method for grid
void Grid::update(sf::Time dt)
{
	diffusePheromones(0.1,0.01, 0,20 );

	// Calculate evaporation rates
	float homeEvaporationRate = pow(homeDecay, dt.asSeconds() );
	float foodEvaporationRate = pow(foodDecay, dt.asSeconds() );

	// Evaporate home and food pheromones
	for (float& cell : m_homePheromone)
	{
		if (cell > 0)
		{
			cell *= homeEvaporationRate;
		}
	}
	for (float& cell : m_foodPheromone)
	{
		if (cell > 0)
		{
			cell *= foodEvaporationRate;
		}
	}
	//add food pheromones in cells that contain food
	for (int i = 0; i < m_foodAmount.size();i++)
	{
		if (m_foodAmount[i] > 0)
		{
			m_foodPheromone[i] = 10000;
		}
	}
}
// Draw method to render the grid
void Grid::draw(sf::RenderWindow& window)
{
	// Draw cells
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			float x = i * m_cellSize;
			float y = j * m_cellSize;

			//draw home pheromone
			float HP = m_homePheromone[gridpos(i,j)];
			if (HP > 1000) HP = 1000;
			HP /= 15;
			if (HP > 255)HP = 255;

			m_homePheromoneColour.a = HP;
			if (HP > 10)
			{
				m_cellShape.setFillColor(m_homePheromoneColour);
				m_cellShape.setPosition(x, y);
				window.draw(m_cellShape);
			}
			
			//draw food pheromone
			float FP = m_foodPheromone[gridpos(i, j)];
			if (FP > 1000) FP = 1000;			
			FP /= 15;
			if (FP > 255)FP = 255;
			m_foodPheromoneColour.a = FP;
			if (FP > 10)
			{
				m_cellShape.setFillColor(m_foodPheromoneColour);

				m_cellShape.setPosition(x, y);
				window.draw(m_cellShape);
			}

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
				m_cellShape.setFillColor(m_wallColors[gridpos(i, j)]);
				m_cellShape.setPosition(x, y);
				window.draw(m_cellShape);
			}
		}
	}
}

// Add home pheromone to the specified cell
void Grid::addHomePheromone(int x, int y, float amount)
{
	if (gridpos(x / m_cellSize, y / m_cellSize) <= m_homePheromone.size())
	{
		m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] += amount;

		if (m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] > 10000)
		{
			m_homePheromone[gridpos(x / m_cellSize, y / m_cellSize)] = 10000;
		}
	}	
}

// Add food pheromone to the specified cell
void Grid::addFoodPheromone(int x, int y, float amount)
{

	if (gridpos(x / m_cellSize, y / m_cellSize) <= m_foodPheromone.size())
	{
		m_foodPheromone[gridpos(x / m_cellSize, y / m_cellSize)] += amount;

		if (m_foodPheromone[gridpos(x / m_cellSize, y / m_cellSize)] > 10000)
		{
			m_foodPheromone[gridpos(x / m_cellSize, y / m_cellSize)] = 10000;
		}
	}
}

// Add food to the specified cell
void Grid::addFood(int x, int y, float amount)
{

	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			
			int nx = x/m_cellSize + dx;
			int ny = y/m_cellSize + dy;
			m_foodAmount[gridpos(nx, ny)] = amount;
			if (nx < 0 || nx >= m_width || ny < 0 || ny >= m_height) continue;
		}
	}
}

// Add walls to the specified cell
void Grid::addWalls(int x, int y,float amount)
{

	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {

			int nx = x / m_cellSize + dx;
			int ny = y / m_cellSize + dy;
			m_walls[gridpos(nx, ny)] = amount;
			if (nx < 0 || nx >= m_width || ny < 0 || ny >= m_height) continue;

		}
	}
}

// Remove food from the specified cell
void Grid::removeFood(int x, int y, float amount)
{
	m_foodAmount[gridpos(x / m_cellSize, y / m_cellSize)] -= amount;
	if (m_foodAmount[gridpos(x / m_cellSize, y / m_cellSize)] < 1)
	{
		m_foodPheromone[gridpos(x / m_cellSize, y / m_cellSize)] = 0;
	}
	

}


// save grid data to file
bool Grid::saveToFile(const std::string& filename,sf::Vector2f position) const
{

	
	std::ofstream file(filename, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open file " << filename << " for writing\n";
		return false;
	}

	// Write dimensions
	file.write(reinterpret_cast<const char*>(&m_width), sizeof(m_width));
	file.write(reinterpret_cast<const char*>(&m_height), sizeof(m_height));

	// Write data
	file.write(reinterpret_cast<const char*>(m_foodAmount.data()), m_foodAmount.size() * sizeof(float));
	file.write(reinterpret_cast<const char*>(m_walls.data()), m_walls.size() * sizeof(float));
	file.write(reinterpret_cast<const char*>(m_wallColors.data()), m_wallColors.size() * sizeof(sf::Color));
	file.write(reinterpret_cast<const char*>(&position), sizeof(position));

	return true;

}

//load grid data from a file
sf::Vector2f Grid::loadFromFile(const std::string& filename, sf::Vector2f position)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open file " << filename << " for reading\n";
		return position;
	}

	// Read dimensions
	int width, height;
	file.read(reinterpret_cast<char*>(&width), sizeof(width));
	file.read(reinterpret_cast<char*>(&height), sizeof(height));

	// Check dimensions match with current grid
	if (width != m_width || height != m_height) {
		std::cerr << "Cannot load grid from " << filename << " because dimensions do not match\n";
		return position;
	}

	// Read data
	file.read(reinterpret_cast<char*>(m_foodAmount.data()), m_foodAmount.size() * sizeof(float));
	file.read(reinterpret_cast<char*>(m_walls.data()), m_walls.size() * sizeof(float));
	file.read(reinterpret_cast<char*>(m_wallColors.data()), m_wallColors.size() * sizeof(sf::Color));
	sf::Vector2f Position;
	file.read(reinterpret_cast<char*>(&Position), sizeof(Position));

	return Position;
}

//return index for the grid at a given X and Y grid coordinate
int Grid::gridpos(int x, int y)
{
	return x + (y * m_width);

}
//return index for the grid at a given X and Y window coordinate
int Grid::windowToGridPos(int x, int y)
{
	return (x / m_cellSize) + (y / m_cellSize) * m_width;
}

// Diffuse pheromones over the grid based on the provided rates and thresholds
void Grid::diffusePheromones(float homeRate, float foodRate, float homeThreshold, float foodThreshold)
{
	// Create new pheromone grids for home and food
	std::vector<float> newHomePheromomne = m_homePheromone;
	std::vector<float> newFoodPheromomne = m_foodPheromone;

	// Iterate through the grid
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			// Skip wall cells
			if (m_walls[gridpos(x, y)]) continue;
			
			// Calculate the total pheromone levels for home and food in the current cell and neighboring cells
			float totalHomeLevel = m_homePheromone[gridpos(x, y)];
			float totalFoodLevel = m_foodPheromone[gridpos(x, y)];
			for (int dx = -1; dx <= 1; dx++) 
			{
				for (int dy = -1; dy <= 1; dy++) 
				{
					//exclude center cell from calculation
					if (dx == 0 && dy == 0) continue;
					int nx = x + dx;
					int ny = y + dy;
					//exclude cell if its out of bounds
					if (nx < 0 || nx >= m_width || ny < 0 || ny >= m_height) continue;

					totalHomeLevel += m_homePheromone[gridpos(nx,ny)];
					totalFoodLevel += m_foodPheromone[gridpos(nx,ny)];
				}
			}

			// Update the pheromone levels for home and food based on the calculated total levels, rates, and thresholds
			if (totalHomeLevel / 9 > homeThreshold)
			{
				newHomePheromomne[gridpos(x, y)] = (1 - homeRate) * newHomePheromomne[gridpos(x, y)] + homeRate * (totalHomeLevel / 9.0f);
			}
			if (totalFoodLevel / 9 > foodThreshold)
			{
				newFoodPheromomne[gridpos(x, y)] = (1 - foodRate) * newFoodPheromomne[gridpos(x, y)] + foodRate * (totalFoodLevel / 9.0f);
			}
		}
	}
	// Update the grid's pheromone levels with the new values
	m_homePheromone = newHomePheromomne;
	m_foodPheromone = newFoodPheromomne;
}
// Convert noise value to grayscale color
sf::Color Grid::getColorFromNoiseValue(float value)
{
	int intensity = static_cast<int>((value + 1.0f) * 128); // Map the value from [-1, 1] to [0, 255]

	return sf::Color(intensity, intensity, intensity);
}


// Check if there's food at the given position
bool Grid::hasfood(int x, int y)
{
	if (gridpos(x / m_cellSize, y / m_cellSize) <= m_homePheromone.size())
	{
		if (m_foodAmount[gridpos(x / m_cellSize, y / m_cellSize)] > 0)
		{
			return true;
		}
	}
	

	return false;
}
// Check if there's a wall at the given position
bool Grid::isWalls(int x, int y)
{

	if (gridpos(x / m_cellSize, y / m_cellSize) <= m_homePheromone.size())
	{
		if (m_walls[gridpos(x / m_cellSize, y / m_cellSize)] > 0)
		{
			return true;
		}
	}
	
	return false;
}
// Calculate the direction of the home pheromone gradient at the given position
sf::Vector2f Grid::getHomePheromoneDirection(int x, int y)
{
	int X = x / m_cellSize;
	int Y = y / m_cellSize;

	sf::Vector2f direction(0.f, 0.f);

	float maxIntensity = 0.f;

	// Check the intensity of the home pheromones in the cells around the ant
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (X + i >= 0 && X + i < m_width &&
				Y + j >= 0 && Y + j < m_height)
			{
				float intensity = m_homePheromone[gridpos(X+i,Y+j)];
				if (intensity > maxIntensity)
				{
					direction = sf::Vector2f(static_cast<float>(i), static_cast<float>(j));
					maxIntensity = intensity;
				}
			}
		}
	}

	// Normalize the direction vector
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.f)
	{
		direction /= length;
	}

	return direction;

}

// Calculate the direction of the food pheromone gradient at the given position
sf::Vector2f Grid::getFoodPheromoneDirection(int x, int y)
{
	int X = x / m_cellSize;
	int Y = y / m_cellSize;

	sf::Vector2f direction(0.f, 0.f);

	float maxIntensity = 0.f;

	// Check the intensity of the food pheromones in the cells around the ant
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (X + i >= 0 && X + i < m_width &&
				Y + j >= 0 && Y + j < m_height)
			{
				float intensity = m_foodPheromone[gridpos(X + i, Y + j)];
				if (intensity > maxIntensity)
				{
					direction = sf::Vector2f(static_cast<float>(i), static_cast<float>(j));
					maxIntensity = intensity;
				}
			}
		}
	}

	// Normalize the direction vector
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.f)
	{
		direction /= length;
	}

	return direction;
}
// Calculate the direction of the home pheromone gradient at the given position but only within a specified angle 
sf::Vector2f Grid::getHomePheromoneDirectionFO(int x, int y, sf::Vector2f velocity, float angle)
{
	int X = x / m_cellSize;
	int Y = y / m_cellSize;

	sf::Vector2f direction(0.f, 0.f);

	float maxIntensity = 0.f;

	// Check the intensity of the home pheromones in the cells around the ant
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (X + i >= 0 && X + i < m_width &&
				Y + j >= 0 && Y + j < m_height)
			{
				sf::Vector2f cellDirection(i, j);
				float cellLength = std::sqrt(cellDirection.x * cellDirection.x + cellDirection.y * cellDirection.y);
				cellDirection /= cellLength;

				float dotProduct = velocity.x * cellDirection.x + velocity.y * cellDirection.y;
				float Angle = std::acos(dotProduct);

				
				if (Angle <= angle) 
				{
					float intensity = m_homePheromone[gridpos(X + i, Y + j)];
					if (intensity > maxIntensity)
					{
						direction = sf::Vector2f(static_cast<float>(i), static_cast<float>(j));
						maxIntensity = intensity;
					}
				}
			}
		}
	}

	// Normalize the direction vector
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.f)
	{
		direction /= length;
	}

	return direction;
}

// Calculate the direction of the food pheromone gradient at the given position but only within a specified angle 
sf::Vector2f Grid::getFoodPheromoneDirectionFO(int x, int y, sf::Vector2f velocity, float angle)
{
	int X = x / m_cellSize;
	int Y = y / m_cellSize;

	sf::Vector2f direction(0.f, 0.f);

	float maxIntensity = 0.f;

	// Check the intensity of the food pheromones in the cells around the ant
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			if (X + i >= 0 && X + i < m_width &&
				Y + j >= 0 && Y + j < m_height)
			{
				sf::Vector2f cellDirection(i, j);
				float cellLength = std::sqrt(cellDirection.x * cellDirection.x + cellDirection.y * cellDirection.y);
				cellDirection /= cellLength;

				float dotProduct = velocity.x * cellDirection.x + velocity.y * cellDirection.y;
				float Angle = std::acos(dotProduct);

				if (Angle <= angle) 
				{
					float intensity = m_foodPheromone[gridpos(X + i, Y + j)];
					if (intensity > maxIntensity)
					{
						direction = sf::Vector2f(static_cast<float>(i), static_cast<float>(j));
						maxIntensity = intensity;
					}
				}
			}
		}
	}

	// Normalize the direction vector
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.f)
	{
		direction /= length;
	}

	return direction;
}
// return a normal vector of a cell if its a wall based on the walls around it, this is used in rebound calculations
sf::Vector2f Grid::getCellNormal(int x, int y)
{
	sf::Vector2f normal(0.f, 0.f);

	if (isWalls(x,y) )
	{
		// Calculate the normal based on the neighboring cells

		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0) continue; // Skip the center cell
				if (isWalls(x + dx, y + dy)) {
					normal += sf::Vector2f(static_cast<float>(-dx), static_cast<float>(-dy));
				}
			}
		}

		// Normalize the resulting vector
		float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
		if (length != 0.f) {
			normal /= length;
		}
	}

	return normal;
}
//finds the nearest empty cell to a given cell
sf::Vector2i Grid::findNearestEmptyCell(int x, int y)
{
	int X = x / m_cellSize;
	int Y = y / m_cellSize;

	for (int radius = 1; radius < std::max(m_width, m_height); radius++)
	{
		for (int i = -radius; i <= radius; i++)
		{
			for (int j = -radius; j <= radius; j++)
			{
				if (X + i >= 0 && X + i < m_width &&
					Y + j >= 0 && Y + j < m_height)
				{
					if (!m_walls[gridpos(X + i, Y + j)])
					{
						return sf::Vector2i((X + i) * m_cellSize, (Y + j) * m_cellSize);
					}
				}
			}
		}
	}

	return sf::Vector2i(x, y); // If no empty cell is found (unlikely), return the current position
}

float Grid::getCellSize()
{
	return m_cellSize;
}

//randomly generates a map using perlin noise 
void Grid::generateMap(float wall_threshold, float food_threshold, int food_radius)
{
	//set parameters of the noise
	FastNoiseLite noise;
	noise.SetSeed(rand());
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.5);

	// Generate walls and food sources using Perlin noise
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			float value = noise.GetNoise(static_cast<float>(x) / 10, static_cast<float>(y) / 10);

			if (value > wall_threshold)
			{
				m_walls[gridpos(x, y)] = 1;
				m_foodAmount[gridpos(x, y)] = 0;
				m_wallColors[gridpos(x, y)] = getColorFromNoiseValue(value);
			}
			else if (value < food_threshold)
			{				
				m_foodAmount[gridpos(x, y)] = 50;
				m_walls[gridpos(x, y)] = 0;
			}
			else
			{
				m_foodAmount[gridpos(x, y)] =0;
				m_walls[gridpos(x, y)] = 0;
			}
			m_homePheromone [gridpos(x, y)]= 0;
			m_foodPheromone[gridpos(x, y)] = 0;
		}
	}

}

//checks if a given position is a valid place for a colony
bool Grid::isPositionValidForColony(int x, int y, int minDistance)
{
	for (int i = -minDistance; i <= minDistance; i++)
	{
		for (int j = -minDistance; j <= minDistance; j++)
		{
			int newX = x + i;
			int newY = y + j;

			if (newX >= 0 && newX < m_width && newY >= 0 && newY < m_height)
			{
				if (m_walls[gridpos(newX, newY)] || m_foodAmount[gridpos(newX, newY)] > 0)
				{
					return false;
				}
			}
		}
	}

	return true;
}
