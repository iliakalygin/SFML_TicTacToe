#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <iostream>
#include <string>

void drawGrid(sf::RenderWindow& window)
{
    // Create grid lines
    sf::RectangleShape vetical_grid_line1({10.f, 620.f});
    vetical_grid_line1.setPosition({200.f, 0.f});
    sf::RectangleShape vetical_grid_line2({10.f, 620.f});
    vetical_grid_line2.setPosition({410.f, 0.f});
    sf::RectangleShape horizontal_grid_line1({620.f, 10.f});
    horizontal_grid_line1.setPosition({0.f, 200.f});
    sf::RectangleShape horizontal_grid_line2({620.f, 10.f});
    horizontal_grid_line2.setPosition({0.f, 410.f});

    // Draw grid
    window.draw(vetical_grid_line1);
    window.draw(vetical_grid_line2);
    window.draw(horizontal_grid_line1);
    window.draw(horizontal_grid_line2);
}

int main()
{
    // Window settings
    const unsigned int SCREEN_WIDTH = 620;
    const unsigned int SCREEN_HEIGHT = 620;
    
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
    const sf::Color RED = sf::Color::Red;
    const sf::Color BLUE = sf::Color::Blue;
    const sf::Color YELLOW = sf::Color::Yellow;
    const sf::Color GREEN = sf::Color::Green;
    const sf::Color BLACK = sf::Color::Black;
    const sf::Color WHITE = sf::Color::White;

    // Set FPS
    const float FPS = 60.f;
    const sf::Time frameTime = sf::seconds(1.f / FPS);
    sf::Clock clock;

    // Field with all placed X's and O's
    std::string field[9];
    // Clicked buttons
    std::string clikedButtons[9];

    // Button configuration
    const float buttonSize = 200.f;
    const float buttonSpacing = 10.f; // gap between grid lines
    std::vector<sf::RectangleShape> buttons;

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Generate 9 buttons in a 3x3 grid
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                sf::RectangleShape button({buttonSize, buttonSize});
                button.setFillColor(BLACK);
                button.setPosition({
                    col * (buttonSize + buttonSpacing),
                    row * (buttonSize + buttonSpacing)
                });
                buttons.push_back(button); // Appends button
            }
        }

        // -----------------------------------------------------------------------------------
        window.clear();

        drawGrid(window);

        // Draw all buttons
        for (const auto& button : buttons)
        {
            window.draw(button);
        }

        window.display();

        // Sleep to maintain 60 FPS
        sf::Time sleepTime = frameTime - clock.getElapsedTime();
        if (sleepTime > sf::Time::Zero)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.asMicroseconds()));
        }
    }
}
