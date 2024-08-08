#include "NetworkMap.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <fstream>

NetworkMap::NetworkMap(const std::vector<Host> &hostlist) : hosts(hostlist)
{
    view = sf::View(sf::FloatRect(0, 0, 800, 600));
    loadFont("../fonts/Roboto-Regular.ttf");
}

void NetworkMap::loadFont(const std::string &fontPath)
{
    std::cerr << "Current working directory: " << std::filesystem::current_path() << "\n";

    std::ifstream file(fontPath);
    if (!file.good())
    {
        std::cerr << "Font file not found: " << fontPath << "\n";
    }
    else
    {
        std::cerr << "Font file found: " << fontPath << "\n";
    }

    if (!font.loadFromFile(fontPath))
    {
        std::cerr << "Failed to load font: " << fontPath << "\n";
    }
    else
    {
        fontLoaded = true;
    }
}

void NetworkMap::draw(sf::RenderWindow &window) {
    window.setView(view);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    if (hosts.empty()) {
        return;
    }

    float centerX = static_cast<float>(window.getSize().x) / 2.0f;
    float centerY = static_cast<float>(window.getSize().y) / 2.0f;

    hostPositions[hosts[0].ip] = sf::Vector2f(centerX, centerY);

    float layerRadius = 200.0f;
    float radiusIncrement = 200.0f;
    float angleIncrement = 360.0f / static_cast<float>(hosts.size() - 1);

    positionHosts(centerX, centerY);

    drawNodes(window);
    drawConnections(window);
    drawHostDetails(window);
}

void NetworkMap::positionHosts(float centerX, float centerY) {
    hostPositions[hosts[0].ip] = sf::Vector2f(centerX, centerY);

    float layerRadius = 200.0f;
    float radiusIncrement = 200.0f;
    float angleIncrement = 360.0f / static_cast<float>(hosts.size() - 1);

    for (size_t i = 1; i < hosts.size(); ++i) {
        float angle = (static_cast<float>(i) - 1) * angleIncrement;
        float x = centerX + layerRadius * static_cast<float>(std::cos(angle * M_PI / 180.0));
        float y = centerY + layerRadius * static_cast<float>(std::sin(angle * M_PI / 180.0));

        hostPositions[hosts[i].ip] = sf::Vector2f(x, y);

        if (i % 8 == 0) {
            layerRadius += radiusIncrement;
        }
    }
}

void NetworkMap::drawNodes(sf::RenderWindow& window) {
    for (const auto& host : hosts) {
        sf::CircleShape node(10.0f);
        node.setOrigin(10.0f, 10.0f);
        node.setFillColor(isNodeHighlighted && hostPositions[host.ip] == highlightedNode ? sf::Color::Red :
                          isNodeHovered && hostPositions[host.ip] == hoveredNode ? sf::Color::Yellow :
                          sf::Color::Green);

        node.setPosition(hostPositions[host.ip]);
        window.draw(node);

        drawPortText(window, host);
    }
}

void NetworkMap::drawPortText(sf::RenderWindow& window, const Host& host) {
    float text_y = hostPositions[host.ip].y + 20.0f;
    for (const auto& port : host.openPorts) {
        sf::Text text;
        if (fontLoaded) {
            text.setFont(font);
        }
        text.setString(port);
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        text.setPosition(hostPositions[host.ip].x, text_y);
        window.draw(text);
        text_y += 20.0f;
    }
}

void NetworkMap::drawConnections(sf::RenderWindow& window) {
    sf::Vector2f centerPos = hostPositions[hosts[0].ip];
    for (const auto& host : hosts) {
        if (host.ip != hosts[0].ip) {
            sf::Vertex line[] = {
                sf::Vertex(centerPos),
                sf::Vertex(hostPositions[host.ip])
            };
            window.draw(line, 2, sf::Lines);
        }
    }
}

void NetworkMap::drawHostDetails(sf::RenderWindow& window) {
    if (selectedHost) {
        std::stringstream ss;
        ss << "Host IP: " << selectedHost->ip << "\n";
        for (const auto& port : selectedHost->openPorts) {
            ss << "Open Port: " << port << "\n";
        }
        sf::Text details;
        if (fontLoaded) {
            details.setFont(font);
        }
        details.setString(ss.str());
        details.setCharacterSize(14);
        details.setFillColor(sf::Color::White);
        details.setPosition(10.0f, 10.0f);
        window.setView(window.getDefaultView());
        window.draw(details);
        window.setView(view);
    }
}

// Handle node selection when the mouse is pressed
void NetworkMap::handleNodeSelection(sf::RenderWindow& window, const sf::Vector2i& mouseCoords) {
    sf::Vector2f mousePos = window.mapPixelToCoords(mouseCoords);
    isNodeHighlighted = false;
    for (const auto& host : hosts) {
        sf::Vector2f nodePos = hostPositions[host.ip];
        sf::CircleShape node(10.0f);
        node.setOrigin(10.0f, 10.0f);
        node.setPosition(nodePos);
        if (node.getGlobalBounds().contains(mousePos)) {
            highlightedNode = nodePos;
            isNodeHighlighted = true;
            selectedHost = const_cast<Host*>(&host);
            std::cout << "Selected host: " << selectedHost->ip << "\n";
            break;
        }
    }
}

// Handle node hover when the mouse is moved
void NetworkMap::handleNodeHover(sf::RenderWindow& window, const sf::Vector2i& mouseCoords) {
    sf::Vector2f mousePos = window.mapPixelToCoords(mouseCoords);
    isNodeHovered = false;
    for (const auto& host : hosts) {
        sf::Vector2f nodePos = hostPositions[host.ip];
        sf::CircleShape node(10.0f);
        node.setOrigin(10.0f, 10.0f);
        node.setPosition(nodePos);
        if (node.getGlobalBounds().contains(mousePos)) {
            hoveredNode = nodePos;
            isNodeHovered = true;
            break;
        }
    }
}