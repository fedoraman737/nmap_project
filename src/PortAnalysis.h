#ifndef PORTANALYSIS_H
#define PORTANALYSIS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Host.h"

class PortAnalysis {
public:
    PortAnalysis(const std::vector<Host>& hosts);
    void draw(sf::RenderWindow &window);
    void handleEvents(sf::RenderWindow &window, const sf::Event &event);

private:
    const std::vector<Host>& hosts;
    sf::Font font;
    bool fontLoaded;

    sf::Text inputLabel;
    sf::Text analyzeButtonText;
    sf::Text resultText;
    sf::RectangleShape inputBox;
    sf::RectangleShape analyzeButton;

    std::string currentInput;

    void loadFont(const std::string& fontPath);
    void handleTextInput(const sf::Event& event);
    void handleMouseClick(const sf::Event& event, sf::RenderWindow &window);
    void analyzeIP(const std::string& ip);
    std::string joinOpenPorts(const std::vector<std::string>& ports);
};

#endif // PORTANALYSIS_H
