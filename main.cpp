#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <iostream>

int main()
{
    // Window settings
    unsigned int SCREEN_WIDTH = 600;
    unsigned int SCREEN_HEIGHT = 600;
    
    // Load window icon
    sf::Image icon;
    if (!icon.loadFromFile("img/icon.png")) {
        std::cerr << "Error: Failed to load icon from 'img/icon.png'" << std::endl;
        return 1;
    }

    // Create window and set icon
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "TicTacToe");
    window.setIcon(icon.getSize(), icon.getPixelsPtr());

    // Colors
    sf::Color RED = sf::Color::Red;
    sf::Color BLUE = sf::Color::Blue;
    sf::Color YELLOW = sf::Color::Yellow;
    sf::Color GREEN = sf::Color::Green;
    sf::Color BLACK = sf::Color::Black;
    sf::Color WHITE = sf::Color::White;

    // Set FPS
    float FPS = 60.f;
    const sf::Time frameTime = sf::seconds(1.f / FPS);
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        window.clear();

        window.display();

        // Sleep to maintain 60 FPS
        sf::Time sleepTime = frameTime - clock.getElapsedTime();
        if (sleepTime > sf::Time::Zero)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.asMicroseconds()));
        }
    }
}
