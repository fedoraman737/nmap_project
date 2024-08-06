#ifndef NETWORK_MAP_H
#define NETWORK_MAP_H

#include <SFML/Graphics.hpp>
#include "NmapParser.h"

class NetworkMap {
public:
    NetworkMap(const std::vector<Host>& hosts);
    void draw(sf::RenderWindow& window);

private:
    std::vector<Host> hosts;
};

#endif // NETWORK_MAP_H
