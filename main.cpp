#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <iostream>
#include <string>
#include <algorithm> // for std::find

void draw_grid(sf::RenderWindow& window)
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

void draw_field(sf::RenderWindow& window, const std::vector<std::pair<std::string, char>>& field)
{
    for (const auto& entry : field)
    {
        std::string pos = entry.first; // get position "01", "21", etc.
        char symbol = entry.second;    // get 'X' or 'O'

        // Convert position to ints
        int row = pos[0] - '0';
        int col = pos[1] - '0';

        // Get center position for drawing
        float x = col * 210.f + 50.f;
        float y = row * 210.f + 50.f;

        if (symbol == 'X')
        {
            sf::RectangleShape line1({282.f, 10.f});
            line1.setFillColor(sf::Color::White);
            line1.setPosition({x-50.f, y-50.f});
            line1.rotate(sf::degrees(45));

            sf::RectangleShape line2({282.f, 10.f});
            line2.setFillColor(sf::Color::White);
            line2.setPosition({x-50.f, y-60.f+210.f});
            line2.rotate(sf::degrees(-45));

            window.draw(line1);
            window.draw(line2);
        }
        else if (symbol == 'O')
        {
            sf::CircleShape circle(100.f);
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineThickness(10.f);
            circle.setOutlineColor(sf::Color::White);
            circle.setPosition({x-50.f, y-50.f});

            window.draw(circle);
        }
    }
}

void draw_winner_line(sf::RenderWindow& window, int type, int cord = 0){
    sf::RectangleShape line;

    if (type == 0){ 
        // vertical
        line.setSize({10.f, 620.f});
        line.setPosition({cord*200 - 105 + (cord-1)*10.f, 0.f});
    } 
    else if (type == 1){ 
        // horizontal
        line.setSize({620.f, 10.f});
        line.setPosition({0.f, cord*200 - 105 + (cord-1)*10.f});
    } 
    else if (type == 2){ 
        // diagonal top-lefts
        line.setSize({900.f, 8.f});
        line.setPosition({0.f, 1.f});
        line.rotate(sf::degrees(45));
    } 
    else if (type == 3){ 
        // diagonal top-right
        line.setSize({900.f, 8.f});
        line.setPosition({1.f, 620.f});
        line.rotate(sf::degrees(-45));
    }

    line.setFillColor(sf::Color::Red);
    window.draw(line);
}

struct WinnerResult {
    char winner;
    int lineType;
    int coord;
};

WinnerResult check_winner(const std::vector<std::pair<std::string, char>>& field)
{
    WinnerResult result{' ', -1, -1};

    char board[3][3] = {{' ', ' ', ' '},
                        {' ', ' ', ' '},
                        {' ', ' ', ' '}};

    for (const auto& entry : field)
    {
        int row = entry.first[0] - '0';
        int col = entry.first[1] - '0';
        board[row][col] = entry.second;
    }

    // Check rows and columns
    for (int i = 0; i < 3; ++i)
    {
        // Row
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
        {
            result = {board[i][0], 1, i + 1}; // horizontal win
            return result;
        }

        // Column
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
        {
            result = {board[0][i], 0, i + 1}; // vertical win
            return result;
        }
    }

    // Diagonal TL-BR
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
    {
        result = {board[0][0], 2, -1};
        return result;
    }

    // Diagonal TR-BL
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
    {
        result = {board[0][2], 3, -1};
        return result;
    }

    // Draw
    if (field.size() == 9)
    {
        result = {'D', -1, -1};
    }

    return result;
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
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "sfmlapp");
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
    std::vector<std::pair<std::string, char>> field;
    // Store already clicked buttons
    std::vector<std::string> clickedButtons;
    // Store current turn
    int turn = 1;

    // set mouse pressed state
    bool wasMousePressed = false;

    // set result state
    bool resultPrinted = false;

    // Button configuration
    const float buttonSize = 200.f;
    const float buttonSpacing = 10.f; // gap between grid lines
    std::vector<sf::RectangleShape> buttons;

    // restart timer
    sf::Clock restartClock;
    bool restartScheduled = false;

    // store last result
    WinnerResult lastResult{' ', -1, -1};

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


    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        // Handle mouse press toggle once per click
        bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        window.clear();

        // Read mouse position
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        //std::cout << "Mouse Position - x: " << mousePosition.x << ", y: " << mousePosition.y << std::endl;

        // Cycle though the grid for mouse events
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                // Check over which button mouse is hovering
                if (mousePosition.x >= col * (buttonSize + buttonSpacing) && 
                    mousePosition.x <= (col+1) * (buttonSize + buttonSpacing) &&
                    mousePosition.y >= row * (buttonSize + buttonSpacing) &&
                    mousePosition.y <= (row+1) * (buttonSize + buttonSpacing))
                {
                    //std::cout << "Mouse Position : " << row << ", " << col << std::endl;

                    std::string currentButton = std::to_string(row) + std::to_string(col);

                    // Check if currentButton is in the clickedButtons list to prevent double clicks
                    bool notAlreadyClicked = std::find(clickedButtons.begin(), clickedButtons.end(), currentButton) == clickedButtons.end();
                    if (isMousePressed && !wasMousePressed && notAlreadyClicked && !resultPrinted)
                    {
                        // Feedback on click
                        std::cout << "Button " << currentButton << " pressed." << std::endl;

                        // TODO: Check for turn and insert based on that X or O into 'field' list
                        char mark = (turn % 2 == 0) ? 'O' : 'X';
                        field.push_back({currentButton, mark});
                        
                        // Append the clicked button to the "blacklist" 
                        clickedButtons.push_back(currentButton);
                        turn++; // Increase turn by 1
                    }
                }
            }
        }

        // Reset the mouse state
        wasMousePressed = isMousePressed;

        // Draw all buttons
        for (const auto& button : buttons)
        {
            window.draw(button);
        }

        // Draw grid and field
        draw_grid(window);
        draw_field(window, field);
        // Check if someone won the game
        WinnerResult result = check_winner(field);
        if (result.winner != ' ' && !resultPrinted) {
            if (result.winner == 'D') {
                std::cout << "It's a Draw!" << std::endl;
            } else {
                std::cout << "\nPlayer " << result.winner << " wins!" << std::endl;
            }
            lastResult = result;
            resultPrinted = true;
            restartScheduled = true;
            restartClock.restart();
        }

        // Always draw winner line while game over
        if (resultPrinted && lastResult.winner != 'D' && lastResult.winner != ' ') {
            draw_winner_line(window, lastResult.lineType, lastResult.coord);
        }

        window.display();

        // reset game after 2 seconds after gameover
        if (restartScheduled && restartClock.getElapsedTime().asSeconds() > 2.f) {
            field.clear();
            clickedButtons.clear();
            turn = 1;
            resultPrinted = false;
            restartScheduled = false;
            std::cout << "Game restarted!" << std::endl;
        }

        
        // Sleep to maintain 60 FPS
        sf::Time sleepTime = frameTime - clock.getElapsedTime();
        if (sleepTime > sf::Time::Zero)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.asMicroseconds()));
        }
    }
}
