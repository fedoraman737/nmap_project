#ifndef NETWORKMAP_H
#define NETWORKMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Host.h"

class NetworkMap
{
public:
    NetworkMap(const std::vector<Host> &hosts);

    void draw(sf::RenderWindow &window);

    sf::View view;

    float zoomFactor;

    std::vector<Host> hosts;
    Host *selectedHost = nullptr;
    std::map<std::string, sf::Vector2f> hostPositions;

    bool isNodeHovered = false;
    bool isNodeHighlighted = false;

    sf::Vector2f hoveredNode;
    sf::Vector2f highlightedNode;

private:
    bool fontLoaded = false;
    sf::Font font;

    sf::FloatRect panLimits;

    void loadFont(const std::string &fontPath);
};

#endif // NETWORKMAP_H
