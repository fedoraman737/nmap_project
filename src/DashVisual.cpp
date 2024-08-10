//
// Created by thehellspy on 08/08/24.
//

// DashVisual.cpp

#include "DashVisual.h"
#include <iostream>
#include <SFML/Graphics.hpp>

DashVisual::DashVisual(float width, float height) {
    // Set up the dashboard background
    dashboard.setSize(sf::Vector2f(width, height));
    dashboard.setFillColor(sf::Color(50, 50, 50));  // Dark grey background

    // Load the font first
    if (!font.loadFromFile("fonts/Roboto-Regular.ttf")) {
        std::cerr << "Failed to load font\n";
        fontLoaded = false;
    } else {
        std::cout << "Font loaded successfully\n";
        fontLoaded = true;
    }

    // Initialize the navigation items with the appropriate text
    initButton(visualizeBtn, "Network Map", 20.0f, true);   // Set the first button as active
    initButton(analyzeBtn, "Port Analysis", 70.0f, false);
    initButton(helpBtn, "Vulnerability Insights", 120.0f, false);

    // Set the first button as active
    visualizeBtn.setFillColor(sf::Color(139, 0, 0));  // Dark red for active button
}

void DashVisual::initButton(sf::RectangleShape &button, const std::string &text, float yPos, bool isActive) {
    button.setSize(sf::Vector2f(180.0f, 40.0f));  // Slightly smaller than the dashboard width
    button.setPosition(10.0f, yPos);

    // Set the button color based on whether it is active or not
    if (isActive) {
        button.setFillColor(sf::Color(139, 0, 0));  // Dark red for active button
    } else {
        button.setFillColor(sf::Color(100, 100, 100));  // Default grey for non-active buttons
    }

    // Text configuration for the button
    sf::Text buttonText;
    if (fontLoaded) {
        buttonText.setFont(font);
    }
    buttonText.setString(text);
    buttonText.setCharacterSize(14);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(button.getPosition().x + 20.0f, button.getPosition().y + 10.0f);  // Center the text
    buttonsText.push_back(buttonText);
    buttons.push_back(button);
}

void DashVisual::loadFont(const std::string &fontPath) {
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Failed to load font from: " << fontPath << "\n";
    } else {
        fontLoaded = true;
        std::cout << "Font loaded successfully from: " << fontPath << "\n";
    }
}

void DashVisual::draw(sf::RenderWindow &window) {
    sf::View originalView = window.getView(); // Store the original view
    window.setView(window.getDefaultView()); // Reset to default view to draw fixed elements

    handleMouseHover(window); // Handle hover effect before drawing

    window.draw(dashboard);

    // Draw the buttons first
    for (auto &button : buttons) {
        window.draw(button);
    }

    // Draw the text on top of the buttons
    if (fontLoaded) {
        for (auto &buttonText : buttonsText) {
            window.draw(buttonText);
        }
    }

    window.setView(originalView); // Restore the original view after drawing fixed elements
}

void DashVisual::handleMouseHover(const sf::RenderWindow &window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    for (auto & button : buttons) {
        if (button.getFillColor() == sf::Color(139, 0, 0)) {
            // Skip the active button to keep its dark red color
            continue;
        }
        if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            button.setFillColor(sf::Color(170, 170, 170)); // Lighter grey for hover effect
        } else {
            button.setFillColor(sf::Color(100, 100, 100)); // Default grey for non-active buttons
        }
    }
}

