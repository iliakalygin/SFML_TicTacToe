#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <iostream>

int main()
{
    // Window settings
    unsigned int SCREEN_WIDTH = 620;
    unsigned int SCREEN_HEIGHT = 620;
    
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

        // Draw Grid
        sf::RectangleShape vetical_grid_line1({10.f, 600.f});
        vetical_grid_line1.setPosition({200.f, 0.f});
        sf::RectangleShape vetical_grid_line2({10.f, 600.f});
        vetical_grid_line2.setPosition({410.f, 0.f});
        sf::RectangleShape horizontal_grid_line1({600.f, 10.f});
        horizontal_grid_line1.setPosition({0.f, 200.f});
        sf::RectangleShape horizontal_grid_line2({600.f, 10.f});
        horizontal_grid_line2.setPosition({0.f, 410.f});


        window.clear();

        // Draw Grid
        window.draw(vetical_grid_line1);
        window.draw(vetical_grid_line2);
        window.draw(horizontal_grid_line1);
        window.draw(horizontal_grid_line2);

        window.display();

        // Sleep to maintain 60 FPS
        sf::Time sleepTime = frameTime - clock.getElapsedTime();
        if (sleepTime > sf::Time::Zero)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.asMicroseconds()));
        }
    }
}
