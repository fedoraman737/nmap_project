#include "NetworkMap.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <filesystem> // For file existence check and current path
#include <fstream> // For file stream

NetworkMap::NetworkMap(const std::vector<Host>& hosts) : hosts(hosts) {
    view = sf::View(sf::FloatRect(0, 0, 800, 600));
    loadFont("../fonts/Roboto-Regular.ttf"); // Relative path from cmake-build-debug
}

void NetworkMap::loadFont(const std::string& fontPath) {
    // Print current working directory
    std::cerr << "Current working directory: " << std::filesystem::current_path() << "\n";

    // Check if the font file exists
    std::ifstream file(fontPath);
    if (!file.good()) {
        std::cerr << "Font file not found: " << fontPath << "\n";
    } else {
        std::cerr << "Font file found: " << fontPath << "\n";
    }

    // Load the Roboto font from the relative path
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Failed to load font: " << fontPath << "\n";
    } else {
        fontLoaded = true;
    }
}

void NetworkMap::draw(sf::RenderWindow& window) {
    window.setView(view);

    if (hosts.empty()) {
        return;
    }

    float angleIncrement = 360.0f / static_cast<float>(hosts.size());
    float radius = 300.0f;
    float centerX = static_cast<float>(window.getSize().x) / 2.0f;
    float centerY = static_cast<float>(window.getSize().y) / 2.0f;

    for (size_t i = 0; i < hosts.size(); ++i) {
        const auto& host = hosts[i];
        sf::CircleShape node(10.0f);
        node.setOrigin(10.0f, 10.0f); // Center the origin
        node.setFillColor(isNodeHighlighted && hostPositions[host.ip] == highlightedNode ? sf::Color::Red :
                          isNodeHovered && hostPositions[host.ip] == hoveredNode ? sf::Color::Yellow :
                          sf::Color::Green);

        float angle = angleIncrement * static_cast<float>(i);
        float x = centerX + radius * std::cos(angle * M_PI / 180.0f);
        float y = centerY + radius * std::sin(angle * M_PI / 180.0f);
        node.setPosition(x, y);
        window.draw(node);

        hostPositions[host.ip] = sf::Vector2f(x, y);

        float text_y = y + 20.0f;
        for (const auto& port : host.openPorts) {
            sf::Text text;
            if (fontLoaded) {
                text.setFont(font); // Set the font
            }
            text.setString(port);
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::White);
            text.setPosition(x, text_y);
            window.draw(text);
            text_y += 20.0f;
        }
    }

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

    // Display details of the selected host
    if (selectedHost) {
        std::stringstream ss;
        ss << "Host IP: " << selectedHost->ip << "\n";
        for (const auto& port : selectedHost->openPorts) {
            ss << "Open Port: " << port << "\n";
        }
        sf::Text details;
        if (fontLoaded) {
            details.setFont(font); // Set the font
        }
        details.setString(ss.str());
        details.setCharacterSize(14);
        details.setFillColor(sf::Color::White);
        details.setPosition(10.0f, 10.0f);
        window.draw(details);
    }
}

void NetworkMap::handleEvents(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        view.zoom(event.mouseWheelScroll.delta > 0 ? 0.9f : 1.1f);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Middle) {
            dragging = true;
            oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        } else if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            isNodeHighlighted = false;
            for (const auto& host : hosts) {
                sf::Vector2f nodePos = hostPositions[host.ip];
                sf::CircleShape node(10.0f);
                node.setOrigin(10.0f, 10.0f); // Center the origin
                node.setPosition(nodePos);
                if (node.getGlobalBounds().contains(mousePos)) {
                    highlightedNode = nodePos;
                    isNodeHighlighted = true;
                    selectedHost = const_cast<Host*>(&host); // Store a pointer to the selected host
                    std::cout << "Selected host: " << selectedHost->ip << "\n"; // Debug output
                    break;
                }
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Middle) {
            dragging = false;
        }
    }
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        isNodeHovered = false;
        for (const auto& host : hosts) {
            sf::Vector2f nodePos = hostPositions[host.ip];
            sf::CircleShape node(10.0f);
            node.setOrigin(10.0f, 10.0f); // Center the origin
            node.setPosition(nodePos);
            if (node.getGlobalBounds().contains(mousePos)) {
                hoveredNode = nodePos;
                isNodeHovered = true;
                break;
            }
        }
        if (dragging) {
            sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            sf::Vector2f deltaPos = oldPos - newPos;
            view.setCenter(view.getCenter() + deltaPos);
            oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        }
    }
}
