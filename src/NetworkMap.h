#ifndef NETWORKMAP_H
#define NETWORKMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include "Host.h"  // Include the Host.h file

class NetworkMap
{
public:
    NetworkMap(const std::vector<Host> &hosts);

    void draw(sf::RenderWindow &window);

    sf::View view;
    std::vector<Host> hosts;
    Host *selectedHost = nullptr;
    std::map<std::string, sf::Vector2f> hostPositions;

    bool isNodeHovered = false;
    bool isNodeHighlighted = false;

    void handleNodeSelection(sf::RenderWindow& window);
    void handleNodeHover(sf::RenderWindow& window);

    sf::Vector2f hoveredNode;
    sf::Vector2f highlightedNode;

private:
    bool fontLoaded = false;
    sf::Font font;
    sf::FloatRect panLimits;


    void loadFont(const std::string& fontPath);
    void positionHosts(float centerX, float centerY);
    void drawNodes(sf::RenderWindow& window);
    void drawPortText(sf::RenderWindow& window, const Host& host);
    void drawConnections(sf::RenderWindow& window);
    void drawHostDetails(sf::RenderWindow& window);
};

#endif // NETWORKMAP_H
