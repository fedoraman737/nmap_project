#include "NetworkMap.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <random>

sf::Vector2i lastPos;

NetworkMap::NetworkMap(const std::vector<Host>& hosts) : hosts(hosts) {
    view = sf::View(sf::FloatRect(0, 0, 800, 600));
    loadFont("fonts/Roboto-Regular.ttf");
    calculatePanLimits();
}

void NetworkMap::loadFont(const std::string& fontPath) {
    std::cerr << "Current working directory: " << std::filesystem::current_path() << "\n";

    std::ifstream file(fontPath);
    if (!file.good()) {
        std::cerr << "Font file not found: " << fontPath << "\n";
    } else {
        std::cerr << "Font file found: " << fontPath << "\n";
    }

    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Failed to load font: " << fontPath << "\n";
    } else {
        fontLoaded = true;
    }
}

void NetworkMap::calculatePanLimits() {
    if (hosts.empty()) return;

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();

    for (const auto& host : hosts) {
        sf::Vector2f pos = hostPositions[host.ip];
        if (pos.x < minX) minX = pos.x;
        if (pos.x > maxX) maxX = pos.x;
        if (pos.y < minY) minY = pos.y;
        if (pos.y > maxY) maxY = pos.y;
    }

    panLimits = sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
}

void NetworkMap::draw(sf::RenderWindow& window) {
    window.setView(view);

    if (hosts.empty()) {
        return;
    }

    // Determine the center of the window (this will be the initial position for the root node)
    float centerX = static_cast<float>(window.getSize().x) / 2.0f;
    float centerY = static_cast<float>(window.getSize().y) / 2.0f;

    // Assume the first host is the gateway/modem and initially position it at the center
    hostPositions[hosts[0].ip] = sf::Vector2f(centerX, centerY);

    // Position the other hosts using the force-directed algorithm
    positionHostsUsingForceDirected(centerX, centerY);

    // Now adjust the view to center it on the root node's actual position
    sf::Vector2f rootPosition = hostPositions[hosts[0].ip];
    view.setCenter(rootPosition);

    // Draw the connections and nodes
    drawConnections(window);
    drawNodes(window);
    drawHostDetails(window);
}

void NetworkMap::positionHostsUsingForceDirected(float centerX, float centerY) {
    const float repulsiveForceStrength = 2000.0f;
    const float damping = 0.85f;
    const int iterations = 50;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-200.0f, 200.0f);

    // Initialize positions randomly with a bias towards different quadrants
    for (size_t i = 1; i < hosts.size(); ++i) {
        float x = centerX + distribution(generator);
        float y = centerY + distribution(generator);

        if (hosts[i].openPorts.empty()) {
            x -= 300; // Bias for purple nodes
            y += 300;
        } else {
            bool hasCommonPorts = false;
            for (const auto& otherHost : hosts) {
                if (hosts[i].ip != otherHost.ip && !otherHost.openPorts.empty()) {
                    for (const auto& port : hosts[i].openPorts) {
                        if (std::find(otherHost.openPorts.begin(), otherHost.openPorts.end(), port) != otherHost.openPorts.end()) {
                            hasCommonPorts = true;
                            break;
                        }
                    }
                    if (hasCommonPorts) break;
                }
            }
            if (hasCommonPorts) {
                x += 300; // Bias for green nodes
                y -= 300;
            } else {
                x -= 300; // Bias for blue nodes
                y -= 300;
            }
        }

        hostPositions[hosts[i].ip] = sf::Vector2f(x, y);
    }

    // Iterate to apply forces
    for (int iter = 0; iter < iterations; ++iter) {
        // Reset forces
        for (auto& host : hosts) {
            forces[host.ip] = sf::Vector2f(0.0f, 0.0f);
        }

        // Calculate repulsive forces between nodes
        for (size_t i = 0; i < hosts.size(); ++i) {
            for (size_t j = i + 1; j < hosts.size(); ++j) {
                sf::Vector2f diff = hostPositions[hosts[i].ip] - hostPositions[hosts[j].ip];
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance == 0) {
                    distance = 0.1f;
                }

                // Repulsive force for all nodes
                sf::Vector2f repulsiveForce = (repulsiveForceStrength / (distance * distance)) * diff / distance;
                forces[hosts[i].ip] += repulsiveForce;
                forces[hosts[j].ip] -= repulsiveForce;
            }
        }

        // Apply forces to positions
        for (auto& host : hosts) {
            hostPositions[host.ip] += forces[host.ip] * damping;
        }
    }

    // Ensure no overlap by adjusting positions slightly if they are too close
    const float minDistance = 60.0f;
    for (size_t i = 0; i < hosts.size(); ++i) {
        for (size_t j = i + 1; j < hosts.size(); ++j) {
            sf::Vector2f diff = hostPositions[hosts[i].ip] - hostPositions[hosts[j].ip];
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (distance < minDistance) {
                sf::Vector2f adjustment = (minDistance - distance) * diff / distance;
                hostPositions[hosts[i].ip] += adjustment / 2.0f;
                hostPositions[hosts[j].ip] -= adjustment / 2.0f;
            }
        }
    }
}


void NetworkMap::drawNodes(sf::RenderWindow& window) {
    for (const auto& host : hosts) {
        float radius = 10.0f + static_cast<float>(host.openPorts.size()) * 2.0f; // size increases with the number of open ports
        sf::CircleShape node(radius);
        node.setOrigin(radius, radius);

        // Determine node color based on ports
        sf::Color nodeColor;
        if (host.ip == hosts[0].ip) {
            nodeColor = sf::Color::White; // Root node -> white color
        } else if (host.openPorts.empty()) {
            nodeColor = sf::Color::Magenta; // No open ports -> purple color
        } else {
            bool hasCommonPorts = false;
            for (const auto& otherHost : hosts) {
                if (host.ip != otherHost.ip && !otherHost.openPorts.empty()) {
                    for (const auto& port : host.openPorts) {
                        if (std::find(otherHost.openPorts.begin(), otherHost.openPorts.end(), port) != otherHost.openPorts.end()) {
                            hasCommonPorts = true;
                            break;
                        }
                    }
                    if (hasCommonPorts) break;
                }
            }
            if (hasCommonPorts) {
                nodeColor = sf::Color::Green; // Common ports -> green color
            } else {
                nodeColor = sf::Color::Blue; // No common ports -> blue color
            }
        }

        // Override color if hovered or selected
        if (isNodeHighlighted && hostPositions[host.ip] == highlightedNode) {
            nodeColor = sf::Color::Red; // Active (selected) node -> red color
        } else if (isNodeHovered && hostPositions[host.ip] == hoveredNode) {
            nodeColor = sf::Color::Yellow; // Hovered node -> yellow color
        }

        node.setFillColor(nodeColor);
        node.setPosition(hostPositions[host.ip]);
        window.draw(node);

        // Draw IP address/hostname with background rectangle
        sf::Text text;
        if (fontLoaded) {
            text.setFont(font);
        }
        text.setString(host.ip); // change this to host.hostname if you have hostname information
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);

        // Calculate text position
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(hostPositions[host.ip].x - textBounds.width / 2.0f, hostPositions[host.ip].y - radius - 25.0f);

        // Draw background rectangle for the text
        sf::RectangleShape textBackground(sf::Vector2f(textBounds.width + 4.0f, textBounds.height + 4.0f));
        textBackground.setFillColor(sf::Color(50, 50, 50)); // background color here
        textBackground.setPosition(text.getPosition().x - 2.0f, text.getPosition().y - 2.0f);

        window.draw(textBackground);
        window.draw(text);

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

        // Set the position to the top right corner
        float textX = static_cast<float>(window.getSize().x) - details.getLocalBounds().width - 10.0f;  // 10px padding from the right
        float textY = 10.0f;  // 10px padding from the top
        details.setPosition(textX, textY);

        window.setView(window.getDefaultView());
        window.draw(details);
        window.setView(view);
    }
}


void NetworkMap::handleEvents(sf::RenderWindow& window, const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        handleZoom(event.mouseWheelScroll.delta);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        handleMousePress(window, event.mouseButton);
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        handleMouseRelease(event.mouseButton);
    }
    if (event.type == sf::Event::MouseMoved) {
        handleMouseMove(window, event.mouseMove);
    }
}

// Handle zooming in and out
void NetworkMap::handleZoom(float delta) {
    float zoomFactor = (delta > 0) ? 0.9f : 1.1f;
    view.zoom(zoomFactor);
    view.setSize(std::max(view.getSize().x, 800.0f), std::max(view.getSize().y, 600.0f));
}

// Handle mouse button press events
void NetworkMap::handleMousePress(sf::RenderWindow& window, const sf::Event::MouseButtonEvent& event) {
    if (event.button == sf::Mouse::Middle) {
        dragging = true;
        oldPos = window.mapPixelToCoords(sf::Vector2i(event.x, event.y));
        lastPos = sf::Mouse::getPosition(window);
        std::cout << "MouseButtonPressed: " << oldPos.x << ", " << oldPos.y << std::endl;
    } else if (event.button == sf::Mouse::Left) {
        handleNodeSelection(window, sf::Vector2i(event.x, event.y));
    }
}

// Handle mouse button release events
void NetworkMap::handleMouseRelease(const sf::Event::MouseButtonEvent& event) {
    if (event.button == sf::Mouse::Middle) {
        dragging = false;
        std::cout << "MouseButtonReleased" << std::endl;
    }
}

// Handle mouse movement events
void NetworkMap::handleMouseMove(sf::RenderWindow& window, const sf::Event::MouseMoveEvent& event) {
    if (dragging) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2i delta = lastPos - mousePos;
        view.move(static_cast<float>(delta.x), static_cast<float>(delta.y));
        lastPos = mousePos;
        std::cout << "MouseMoved: " << mousePos.x << ", " << mousePos.y << " Delta: " << delta.x << ", " << delta.y << std::endl;
    } else {
        handleNodeHover(window, sf::Vector2i(event.x, event.y));
    }
}

// Handle node selection when the mouse is pressed
void NetworkMap::handleNodeSelection(sf::RenderWindow& window, const sf::Vector2i& mouseCoords) {
    sf::Vector2f mousePos = window.mapPixelToCoords(mouseCoords);
    isNodeHighlighted = false;

    for (const auto& host : hosts) {
        sf::Vector2f nodePos = hostPositions[host.ip];
        float radius = 10.0f + static_cast<float>(host.openPorts.size()) * 2.0f; // match the radius used in drawNodes

        // Calculate the distance between the mouse position and the center of the node
        sf::Vector2f diff = mousePos - nodePos;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (distance <= radius) {
            highlightedNode = nodePos;
            isNodeHighlighted = true;
            selectedHost = const_cast<Host*>(&host);
            std::cout << "Selected host: " << selectedHost->ip << "\n";
            break;
        }
    }
}


void NetworkMap::handleNodeHover(sf::RenderWindow& window, const sf::Vector2i& mouseCoords) {
    sf::Vector2f mousePos = window.mapPixelToCoords(mouseCoords);
    isNodeHovered = false;

    for (const auto& host : hosts) {
        sf::Vector2f nodePos = hostPositions[host.ip];
        float radius = 10.0f + static_cast<float>(host.openPorts.size()) * 2.0f; // Match the radius used in drawNodes

        // Calculate the distance between the mouse position and the center of the node
        sf::Vector2f diff = mousePos - nodePos;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (distance <= radius) {
            hoveredNode = nodePos;
            isNodeHovered = true;
            break;
        }
    }
}