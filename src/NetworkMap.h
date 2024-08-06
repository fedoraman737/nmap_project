#ifndef NETWORK_MAP_H
#define NETWORK_MAP_H

#include <SFML/Graphics.hpp>
#include "NmapParser.h"
#include <map>
#include <string>
#include <vector>

class NetworkMap {
public:
    NetworkMap(const std::vector<Host>& hosts);
    void draw(sf::RenderWindow& window);
    void handleEvents(sf::RenderWindow& window, sf::Event& event);

private:
    void loadFont(const std::string& fontPath);

    std::vector<Host> hosts;
    sf::View view;
    sf::Font font;
    bool fontLoaded = false;
    std::map<std::string, sf::Vector2f> hostPositions;
    sf::Vector2f highlightedNode;
    sf::Vector2f hoveredNode;
    bool isNodeHighlighted = false;
    bool isNodeHovered = false;
    Host* selectedHost = nullptr;
    bool dragging = false;
    sf::Vector2f oldPos;
};

#endif // NETWORK_MAP_H
