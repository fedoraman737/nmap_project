//
// Created by thehellspy on 08/08/24.
//

// DashVisual.h

#ifndef DASHVISUAL_H
#define DASHVISUAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DashVisual {
public:
    DashVisual(float width, float height);

    void loadFont(const std::string &fontPath);
    void draw(sf::RenderWindow &window);

    // New methods for managing active buttons
    void handleMouseClick(sf::RenderWindow &window);
    void setActiveButton(sf::RectangleShape &button);
    bool isActiveButton(const std::string &buttonText) const;

private:
    sf::RectangleShape dashboard;
    sf::RectangleShape visualizeBtn;
    sf::RectangleShape analyzeBtn;
    sf::RectangleShape helpBtn;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonsText;
    sf::Font font;
    bool fontLoaded = false;

    // Store the active button's text
    std::string activeButtonText;

    void initButton(sf::RectangleShape &button, const std::string &text, float yPos, bool isActive);
    void handleMouseHover(const sf::RenderWindow &window);
};

#endif // DASHVISUAL_H
