#include "NetworkMap.h"
#include <cmath>
#include <map>

NetworkMap::NetworkMap(const std::vector<Host>& hosts) : hosts(hosts) {
    view = sf::View(sf::FloatRect(0, 0, 800, 600));
}

void NetworkMap::draw(sf::RenderWindow& window) {
    window.setView(view);

    float angleIncrement = 360.0f / hosts.size();
    float radius = 300.0f;
    float centerX = window.getSize().x / 2;
    float centerY = window.getSize().y / 2;

    std::map<std::string, sf::Vector2f> hostPositions;

    for (size_t i = 0; i < hosts.size(); ++i) {
        const auto& host = hosts[i];
        sf::CircleShape node(10);
        node.setFillColor(sf::Color::Green);
        float angle = angleIncrement * i;
        float x = centerX + radius * cos(angle * M_PI / 180.0f);
        float y = centerY + radius * sin(angle * M_PI / 180.0f);
        node.setPosition(x, y);
        window.draw(node);

        hostPositions[host.ip] = sf::Vector2f(x, y);

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

    if (!hosts.empty()) {
        sf::Vector2f centerPos = hostPositions[hosts[0].ip];
        for (const auto& [ip, position] : hostPositions) {
            if (ip != hosts[0].ip) {
                sf::Vertex line[] = {
                    sf::Vertex(centerPos),
                    sf::Vertex(position)
                };
                window.draw(line, 2, sf::Lines);
            }
        }
    }
}

void NetworkMap::handleEvents(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0)
            view.zoom(0.9f);
        else
            view.zoom(1.1f);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Middle) {
            dragging = true;
            oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Middle) {
            dragging = false;
        }
    }
    if (event.type == sf::Event::MouseMoved) {
        if (dragging) {
            sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            sf::Vector2f deltaPos = oldPos - newPos;
            view.setCenter(view.getCenter() + deltaPos);
            oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        }
    }
}
