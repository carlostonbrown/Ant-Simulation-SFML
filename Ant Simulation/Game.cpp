#include "Game.h"
#include <ctime>
#include <iostream>

//set the framerate
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 144.f);

// Game constructor
Game::Game()
    : window(sf::VideoMode(width, height), "Ant Simulation"),
    m_colony(sf::Vector2f(width / 2, height / 2), 1000, sf::Color::Red),
    m_grid(width, height, cellsize, sf::Color::Blue, sf::Color::Cyan)
{
}

// Main game loop
void Game::Run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    // Continue running while the window is open
    while (window.isOpen())
    {

        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();

            try
            {
                if (running) update(TimePerFrame * m_simSpeed);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception caught: " << e.what() << std::endl;
                
            }
            
        }
        
        render();
    }

}

// Process window events, such as mouse clicks and key presses
void Game::processEvents()
{
    sf::Event event{};

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            // Add food or walls based on the pressed mouse button
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_grid.addFood(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 100);
            }
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
                m_grid.addWalls(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,1);
            }
            else if (event.mouseButton.button == sf::Mouse::Middle)
            {
                m_grid.addFood(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 0);
                m_grid.addWalls(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,0);
            }
            break;
            // Add food or walls based on the pressed mouse button while moving the mouse
        case sf::Event::MouseMoved:

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_grid.addFood(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 100);

            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                m_grid.addWalls(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,1);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            {
                m_grid.addFood(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 0);
                m_grid.addWalls(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 0);
            }
            break;
            // Handle key presses and releases
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;
            // Close the window
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }

    }
}
// Update the game state based on the elapsed time
void Game::update(sf::Time deltaTime)
{
    m_colony.update(deltaTime,m_grid,width,height);
    m_grid.update(deltaTime);


}

// Render the game elements
void Game::render()
{
    window.clear();
    m_grid.draw(window);
    m_colony.draw(window);

    window.display();
}

// Handle player input (key presses)
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (isPressed) 
    {
        // Increase simulation speed
        if (key == sf::Keyboard::Period)
        {
            if (m_simSpeed < 4)
            {
                m_simSpeed++;
            }

        }
        // Decrease simulation speed
        if (key == sf::Keyboard::Comma)
        {
            if (m_simSpeed > 0)
            {
                m_simSpeed--;
            }
        }
        // Start the simulation
        if (key == sf::Keyboard::Space)
        {
            running = true;
        }
        // Save the grid state to a file
        if (key == sf::Keyboard::S)
        {
            m_grid.saveToFile("grid.bin",m_colony.getPosition());
        }
        // Load the grid state from a file
        if (key == sf::Keyboard::L)
        {
           m_colony = AntColony(m_grid.loadFromFile("grid.bin", m_colony.getPosition()), 500, sf::Color::Red);
           
        }
        // Generate a new map
        if (key == sf::Keyboard::G)
        {
            m_grid.generateMap(0.4, -0.9, 3.0f);

            sf::Vector2f colonyPosition;
            bool validPosition = false;

            // Find a valid random position for the colony
            while (!validPosition)
            {
                int randomX = rand() % width;
                int randomY = rand() % height;
                
                if (m_grid.isPositionValidForColony(randomX / cellsize, randomY / cellsize, 5))
                {
                    colonyPosition = sf::Vector2f(static_cast<float>(randomX), static_cast<float>(randomY));
                    validPosition = true;
                }
            }

            m_colony = AntColony(colonyPosition, 1000, sf::Color::Red);
        }
    }



    
}
