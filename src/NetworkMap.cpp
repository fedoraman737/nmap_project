#include "NetworkMap.h"
#include <cmath>

NetworkMap::NetworkMap(const std::vector<Host>& hosts) : hosts(hosts) {}

void NetworkMap::draw(sf::RenderWindow& window) {
    float angleIncrement = 360.0f / hosts.size();
    float radius = 200.0f; // Radius for the circular layout
    float centerX = window.getSize().x / 2;
    float centerY = window.getSize().y / 2;

    for (size_t i = 0; i < hosts.size(); ++i) {
        const auto& host = hosts[i];
        sf::CircleShape node(10);
        node.setFillColor(sf::Color::Green);
        float angle = angleIncrement * i;
        float x = centerX + radius * cos(angle * M_PI / 180.0f);
        float y = centerY + radius * sin(angle * M_PI / 180.0f);
        node.setPosition(x, y);
        window.draw(node);

        float text_y = y + 20.0f;
        for (const auto& port : host.openPorts) {
            sf::Text text;
            text.setString(port);
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::White);
            text.setPosition(x, text_y);
            window.draw(text);
            text_y += 20.0f;
        }
    }
}
