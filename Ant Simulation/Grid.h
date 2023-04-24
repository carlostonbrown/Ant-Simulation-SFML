#include <SFML/Graphics.hpp>
#include <vector>
#include "FastNoiseLite.h"
#pragma once
class Grid
{
public:

	Grid(int width, int height, int cellSize, sf::Color homePheromoneColour, sf::Color foodPheromoneColour);
	void update(sf::Time dt);
	void draw(sf::RenderWindow& window);
	void addHomePheromone(int x,int y, float amount);
	void addFoodPheromone(int x, int y, float amount);
	void addFood(int x, int y, float amount);
	void addWalls(int x, int y,float amount);
	void removeFood(int x, int y, float amount);
	bool saveToFile(const std::string& filename) const;
	bool loadFromFile(const std::string& filename);
	int windowToGridPos(int x, int y);
	bool hasfood(int x, int y);
	bool isWalls(int x, int y);
	sf::Vector2f getHomePheromoneDirection(int x, int y);
	sf::Vector2f getFoodPheromoneDirection(int x, int y);
	sf::Vector2f getHomePheromoneDirectionFO(int x, int y, sf::Vector2f velocity, float angle);
	sf::Vector2f getFoodPheromoneDirectionFO(int x, int y, sf::Vector2f velocity, float angle);
	sf::Vector2f getCellNormal(int x, int y);
	sf::Vector2i findNearestEmptyCell(int x, int y);
	float getCellSize();
	void generateMap(float wall_threshold, float food_threshold, int food_radius);
	bool isPositionValidForColony(int x, int y, int minDistance);

private:

	float foodDecay = 0.50f;
	float homeDecay = 0.90f;
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
	std::vector<sf::Color>m_wallColors;

	int gridpos(int x, int y);
	void diffusePheromones(float homeRate,float foodRate, float homeThreshold, float foodThreshold);
	sf::Color getColorFromNoiseValue(float value);


	

};