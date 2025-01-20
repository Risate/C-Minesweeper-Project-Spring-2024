/*
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;


// ADD SETTEXT FUNCTION FROM FILES
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

class Tile {
    int x;
    int y;
    string state; //State = hidden, shown, count
    int count; //Count = 0-8
    bool bomb; //Bomb = true, false

public:
    Tile(int xcord, int ycord, string stateE, int countE, bool bombE) : x(xcord),
                                                                        y(ycord), state(stateE), count(countE), bomb(bombE) {

    }
    void Display(){

    }


};

int main() {

    //Isaac Probst's submission for COP3503  Project 3 Milestone 1

    //WELCOME WINDOW
    sf::RenderWindow window(sf::VideoMode(800, 612), "Minesweeper"); // Window and title
    sf::RectangleShape shape(sf::Vector2f(800.f, 612.f)); // Background of welcome window
    shape.setFillColor(sf::Color::Blue); // Set background to blue
    sf::Text welcome; // Text object for welcome window
    string welcomeTxt = "WELCOME TO MINESWEEPER!"; // Welcome text
    welcome.setString(welcomeTxt); // Set welcome to string
    welcome.setCharacterSize(24); // Set size to 24
    welcome.setStyle(sf::Text::Bold);
    welcome.setStyle(sf::Text::Underlined);
    sf::Font font; // Establish a font
    font.loadFromFile("font.ttf"); // Set font to text file font
    welcome.setFont(font); // Apply font to welcome
    welcome.setPosition(250, 150); // Set font coordinates
    sf::Text name; // Text object for welcome window
    string nameTxt = "Enter your name:"; // Welcome text
    name.setString(nameTxt);
    name.setCharacterSize(16);
    name.setFont(font);
    name.setPosition(320, 230);
    name.setStyle(sf::Text::Bold);
    std::string inputname = "|";
    sf::Text displayName;
    displayName.setFont(font);
    displayName.setPosition(320, 290);
    displayName.setCharacterSize(18);
    displayName.setFillColor(sf::Color::Yellow);
    bool ent = false;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                setText(displayName, 400, 290);
                //if not is alphabetical IGNORE
                // if is backspace inputname.pop back and ignore the character
                // if not first and uppercase set to lowercase
                // if ten ignore
                if (!isalpha(event.text.unicode)) {
                    if(event.text.unicode == 8) {
                        if (inputname.length() <= 2) {
                            inputname = "|";
                            displayName.setString(inputname);
                        } else {
                            inputname.pop_back();
                            inputname.pop_back();
                            inputname += "|";
                            displayName.setString(inputname);
                        }
                    }
                    if(event.text.unicode == 13) {
                        if(inputname.length() > 1){
                            window.close();
                            ent = true;
                        }
                    }
                } else if (inputname.length() > 10 ) {
                    if (event.text.unicode == 8) {
                        inputname.pop_back();
                        inputname.pop_back();
                        inputname += "|";
                        displayName.setString(inputname);
                    }
                } else if (inputname.length() == 1 ) {
                    inputname.pop_back();
                    inputname += toupper(static_cast<char>(event.text.unicode));
                    inputname += "|";
                    displayName.setString(inputname);

                } else {
                    inputname.pop_back();
                    inputname += tolower(static_cast<char>(event.text.unicode));
                    inputname += "|";
                    displayName.setString(inputname);
                }
                // after changed, call setText function!!
            }
        }
        window.clear();
        window.draw(shape);
        window.draw(welcome);
        window.draw(name);
        window.draw(displayName);
        window.display();
    }

    if(ent) {
        //GAME WINDOW
        sf::RenderWindow gamewin(sf::VideoMode(800, 612), "Minesweeper"); // Window and title
        sf::RectangleShape rect(sf::Vector2f(800.f, 612.f)); // Background of game window
        rect.setFillColor(sf::Color::Black); // Set background to black

        while (gamewin.isOpen()) {
            sf::Event event;
            while (gamewin.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gamewin.close();
                } else if (event.type == sf::Event::TextEntered) {

                } else if (event.type == sf::Event::TextEntered) {

                }



                gamewin.clear();
                gamewin.draw(rect);
                gamewin.display();
            }
        }
    }

    //LEADERBOARD WINDOW

    return 0;
};
*/