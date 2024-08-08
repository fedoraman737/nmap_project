#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Host.h"
#include "NetworkMap.h"

class EventHandler
{
public:
    EventHandler(sf::RenderWindow &window, NetworkMap &networkMap);
    void handle(sf::Event &event);

private:
    void mouseMoved(sf::Event &event);
    void mouseButtonReleased(sf::Event &event);
    void mouseButtonPressed(sf::Event &event);
    void mouseWheelScrolled(sf::Event &event);

    NetworkMap &networkMap;
    sf::RenderWindow &window;

    bool dragging;
    sf::Vector2i lastPos;
};

#endif // NETWORKMAP_H
