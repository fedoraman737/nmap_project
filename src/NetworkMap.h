#ifndef NETWORKMAP_H
#define NETWORKMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Host.h"

class NetworkMap {
public:
    NetworkMap(const std::vector<Host>& hosts);

    void draw(sf::RenderWindow& window); // Removed const
    void handleEvents(sf::RenderWindow& window, const sf::Event& event);

private:
    std::vector<Host> hosts;
    sf::View view;
    bool dragging = false;
    bool isNodeHovered = false;
    bool isNodeHighlighted = false;
    sf::Vector2f oldPos;
    sf::Vector2f hoveredNode;
    sf::Vector2f highlightedNode;
    Host* selectedHost = nullptr;
    std::map<std::string, sf::Vector2f> hostPositions;
    sf::Font font;
    bool fontLoaded = false;
    sf::FloatRect panLimits;

    void loadFont(const std::string& fontPath);
    void calculatePanLimits();
};

#endif // NETWORKMAP_H
