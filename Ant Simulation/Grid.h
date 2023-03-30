#include <SFML/Graphics.hpp>
#include <vector>

#pragma once
class Grid
{
public:

	Grid(int width, int height, int cellSize, sf::Color homePheromoneColour, sf::Color foodPheromoneColour);
	void update(sf::Time dt);
	void draw(sf::RenderWindow& window);
	void addHomePheromone(int x,int y, float amount);
	void addFood(int x, int y, float amount);
	void addWalls(int x, int y);
	int windowToGridPos(int x, int y);
private:

	float decay = 1;
	int m_width;
	int m_height;
	int m_cellSize;
	sf::RectangleShape m_cellShape;
	sf::Color m_homePheromoneColour;
	sf::Color m_foodPheromoneColour;

	std::vector<float> m_foodPheromone;
	std::vector<float> m_homePheromone;
	std::vector<float> m_foodAmount;
	std::vector<float> m_walls;

	int gridpos(int x, int y);
	void diffusePheromones(float rate);


};