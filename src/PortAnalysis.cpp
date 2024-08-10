#include "PortAnalysis.h"
#include <sstream>
#include <iostream>

PortAnalysis::PortAnalysis(const std::vector<Host>& hosts) : hosts(hosts), currentInput("") {
    loadFont("fonts/Roboto-Regular.ttf");

    // Initialize the input box
    inputBox.setSize(sf::Vector2f(200, 30));
    inputBox.setFillColor(sf::Color(255, 255, 255));

    // Initialize the input label
    if (fontLoaded) {
        inputLabel.setFont(font);
        inputLabel.setCharacterSize(16);
        inputLabel.setFillColor(sf::Color::Black);
        inputLabel.setString("");
    }

    // Initialize the analyze button
    analyzeButton.setSize(sf::Vector2f(100, 30));
    analyzeButton.setFillColor(sf::Color(100, 100, 250));

    // Initialize the analyze button text
    if (fontLoaded) {
        analyzeButtonText.setFont(font);
        analyzeButtonText.setCharacterSize(16);
        analyzeButtonText.setFillColor(sf::Color::White);
        analyzeButtonText.setString("Analyze");
    }

    // Initialize the result text
    if (fontLoaded) {
        resultText.setFont(font);
        resultText.setCharacterSize(16);
        resultText.setFillColor(sf::Color::White);
        resultText.setString("");
    }
}

void PortAnalysis::loadFont(const std::string& fontPath) {
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Failed to load font from: " << fontPath << "\n";
        fontLoaded = false;
    } else {
        fontLoaded = true;
        std::cout << "Font loaded successfully from: " << fontPath << "\n";
    }
}

void PortAnalysis::draw(sf::RenderWindow &window) {
    window.setView(window.getDefaultView()); // Reset the view to default
    window.clear(sf::Color::Black); // Clear the window with a black color

    // Center the input box and button
    sf::Vector2u windowSize = window.getSize();
    float centerX = windowSize.x / 2.0f;

    inputBox.setPosition(centerX - inputBox.getSize().x - 10, windowSize.y / 2.0f - inputBox.getSize().y / 2.0f);
    inputLabel.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + 5);

    analyzeButton.setPosition(centerX + 10, windowSize.y / 2.0f - analyzeButton.getSize().y / 2.0f);
    analyzeButtonText.setPosition(analyzeButton.getPosition().x + 10, analyzeButton.getPosition().y + 5);

    // Position the result text below the input box and button
    resultText.setPosition(centerX - resultText.getLocalBounds().width / 2.0f, analyzeButton.getPosition().y + 50);

    // Draw the input box, button, and result text
    window.draw(inputBox);
    window.draw(inputLabel);
    window.draw(analyzeButton);
    window.draw(analyzeButtonText);
    window.draw(resultText);
}

void PortAnalysis::handleEvents(sf::RenderWindow &window, const sf::Event &event) {
    if (event.type == sf::Event::TextEntered) {
        handleTextInput(event);
    } else if (event.type == sf::Event::MouseButtonPressed) {
        handleMouseClick(event, window);
    }
}

void PortAnalysis::handleTextInput(const sf::Event& event) {
    // Handle backspace
    if (event.text.unicode == 8 && !currentInput.empty()) {
        currentInput.pop_back();
    }
    // Handle regular character input
    else if (event.text.unicode < 128 && event.text.unicode != 8) {
        currentInput += static_cast<char>(event.text.unicode);
    }

    inputLabel.setString(currentInput);
}

void PortAnalysis::handleMouseClick(const sf::Event& event, sf::RenderWindow &window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Check if the analyze button is clicked
    if (analyzeButton.getGlobalBounds().contains(mousePos)) {
        analyzeIP(currentInput);
    }
}

void PortAnalysis::analyzeIP(const std::string& ip) {
    std::vector<std::string> openPorts;

    for (const auto& host : hosts) {
        if (host.ip == ip) {
            openPorts = host.openPorts;
            break;
        }
    }

    if (openPorts.empty()) {
        resultText.setString("No open ports found for IP: " + ip);
    } else {
        resultText.setString("Open ports for IP: " + ip + " -> " + joinOpenPorts(openPorts));
    }
}

std::string PortAnalysis::joinOpenPorts(const std::vector<std::string>& ports) {
    std::string result;
    for (size_t i = 0; i < ports.size(); ++i) {
        if (i > 0) {
            result += ", "; // Separate ports with a comma
        }
        result += ports[i];
    }
    return result;
}
