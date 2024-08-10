#ifndef NETWORKMAP_H
#define NETWORKMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include "Host.h"  // Include the Host.h file

class NetworkMap {
public:
    explicit NetworkMap(const std::vector<Host>& hosts);

    void draw(sf::RenderWindow& window);
    void handleEvents(sf::RenderWindow& window, const sf::Event& event);

private:
    std::vector<Host> hosts;
    sf::View view;
    sf::Font font;
    bool fontLoaded = false;
    bool dragging = false;
    sf::Vector2f oldPos;
    bool isNodeHighlighted = false;
    sf::Vector2f highlightedNode;
    bool isNodeHovered = false;
    sf::Vector2f hoveredNode;
    Host* selectedHost = nullptr;
    sf::FloatRect panLimits;
    std::unordered_map<std::string, sf::Vector2f> hostPositions;
    std::unordered_map<std::string, sf::Vector2f> forces; // declare forces

    void loadFont(const std::string& fontPath);
    void calculatePanLimits();
    void positionHostsHierarchically(float centerX, float centerY, float layerRadius, float radiusIncrement);
    void drawNodes(sf::RenderWindow& window);
    void drawPortText(sf::RenderWindow& window, const Host& host);
    void drawConnections(sf::RenderWindow& window);
    void drawHostDetails(sf::RenderWindow& window);
    void handleZoom(float delta);
    void handleMousePress(sf::RenderWindow& window, const sf::Event::MouseButtonEvent& event);
    void handleMouseRelease(const sf::Event::MouseButtonEvent& event);
    void handleMouseMove(sf::RenderWindow& window, const sf::Event::MouseMoveEvent& event);
    void handleNodeSelection(sf::RenderWindow& window, const sf::Vector2i& mouseCoords);
    void handleNodeHover(sf::RenderWindow& window, const sf::Vector2i& mouseCoords);
    void positionHostsUsingForceDirected(float centerX, float centerY);
};

#endif // NETWORKMAP_H
