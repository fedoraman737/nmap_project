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

    float centerX = static_cast<float>(window.getSize().x) / 2.0f;
    float centerY = static_cast<float>(window.getSize().y) / 2.0f;

    // Central node
    hostPositions[hosts[0].ip] = sf::Vector2f(centerX, centerY);

    // Calculate positions for other hosts
    float layerRadius = 200.0f; // Radius of the first layer
    float radiusIncrement = 200.0f; // Increment for each new layer
    float angleIncrement = 360.0f / (hosts.size() - 1); // Angle increment for each node

    for (size_t i = 1; i < hosts.size(); ++i) {
        float angle = (i - 1) * angleIncrement;
        float x = centerX + layerRadius * std::cos(angle * M_PI / 180.0f);
        float y = centerY + layerRadius * std::sin(angle * M_PI / 180.0f);

        hostPositions[hosts[i].ip] = sf::Vector2f(x, y);

        if (i % 8 == 0) { // Increase radius every 8 nodes
            layerRadius += radiusIncrement;
        }
    }

    // Draw nodes and connections
    for (const auto& host : hosts) {
        sf::CircleShape node(10.0f);
        node.setOrigin(10.0f, 10.0f); // Center the origin
        node.setFillColor(isNodeHighlighted && hostPositions[host.ip] == highlightedNode ? sf::Color::Red :
                          isNodeHovered && hostPositions[host.ip] == hoveredNode ? sf::Color::Yellow :
                          sf::Color::Green);

        node.setPosition(hostPositions[host.ip]);
        window.draw(node);

        float text_y = hostPositions[host.ip].y + 20.0f;
        for (const auto& port : host.openPorts) {
            sf::Text text;
            if (fontLoaded) {
                text.setFont(font); // Set the font
            }
            text.setString(port);
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::White);
            text.setPosition(hostPositions[host.ip].x, text_y);
            window.draw(text);
            text_y += 20.0f;
        }
    }

    // Draw lines connecting each host to the central node
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
        details.setCharacterSize(14 * (1.0f / view.getSize().x * window.getSize().x)); // Adjust size based on zoom level
        details.setFillColor(sf::Color::White);
        details.setPosition(10.0f, 10.0f);
        window.draw(details);
    }
}

void NetworkMap::handleEvents(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
        view.zoom(zoomFactor);
        view.setSize(std::max(view.getSize().x, 800.0f), std::max(view.getSize().y, 600.0f)); // Restrict minimum zoom level
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
            view.move(deltaPos);
            oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        }
    }
    // Restrict panning within bounds
    sf::Vector2f viewCenter = view.getCenter();
    viewCenter.x = std::max(viewCenter.x, 400.0f);
    viewCenter.y = std::max(viewCenter.y, 300.0f);
    viewCenter.x = std::min(viewCenter.x, static_cast<float>(window.getSize().x) - 400.0f);
    viewCenter.y = std::min(viewCenter.y, static_cast<float>(window.getSize().y) - 300.0f);
    view.setCenter(viewCenter);
}
