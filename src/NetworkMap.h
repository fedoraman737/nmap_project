#ifndef NETWORK_MAP_H
#define NETWORK_MAP_H

#include <SFML/Graphics.hpp>
#include "NmapParser.h"

class NetworkMap {
public:
    NetworkMap(const std::vector<Host>& hosts);
    void draw(sf::RenderWindow& window);
    void handleEvents(sf::RenderWindow& window, sf::Event& event);

private:
    std::vector<Host> hosts;
    sf::View view;
    bool dragging = false;
    sf::Vector2f oldPos;
};

#endif // NETWORK_MAP_H
