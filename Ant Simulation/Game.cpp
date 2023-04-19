#include "Game.h"
#include <ctime>
#include <iostream>
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 144.f);


Game::Game()
    : window(sf::VideoMode(width, height), "Ant Simulation"),
    m_colony(sf::Vector2f(width / 2, height / 2), 500, sf::Color::Red),
    m_grid(width, height, 10, sf::Color::Blue, sf::Color::Cyan)
{
    

    


}

void Game::Run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

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

void Game::processEvents()
{
    sf::Event event{};

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
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

        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }

    }
}

void Game::update(sf::Time deltaTime)
{
    m_colony.update(deltaTime,m_grid,width,height);
    m_grid.update(deltaTime);


}

void Game::render()
{
    window.clear();
    m_grid.draw(window);
    m_colony.draw(window);

    window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{


    if (key == sf::Keyboard::Period)
    {
        if (m_simSpeed < 4)
        {
            m_simSpeed++;
        }
       
    }
    if (key == sf::Keyboard::Comma)
    {
        if (m_simSpeed > 0)
        {
            m_simSpeed--;
        }
    }
    if (key == sf::Keyboard::Space)
    {
        running = true;
    }
    if (key == sf::Keyboard::S)
    {
        m_grid.saveToFile("grid.bin");
    }
    if (key == sf::Keyboard::L)
    {
        m_grid.loadFromFile("grid.bin");
    }
}
