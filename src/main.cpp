#include <SFML/Graphics.hpp>
#include "NmapParser.h"
#include "NetworkMap.h"
#include "FileSelector.h"
#include "DashVisual.h"
#include <iostream>
#include <exception>
#include <fstream>
#include "PortAnalysis.h"

int main() {
    try {
        std::cout << "Starting file selection..." << std::endl;
        select_nmap_file();

        std::ifstream file("selected_file.txt");
        std::string nmapFilePath;
        if (!file || !(file >> nmapFilePath)) {
            std::cerr << "Failed to read the selected Nmap file. Exiting..." << std::endl;
            return 1;
        }

        std::remove("selected_file.txt");

        NmapParser parser;
        std::vector<Host> hosts = parser.parseNmapXML(nmapFilePath);

        if (hosts.empty()) {
            std::cerr << "No hosts found in the Nmap XML file. Exiting..." << std::endl;
            return 1;
        }

        // Create the window with full screen width and height in windowed mode
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height), "Network Visualizer with Dashboard", sf::Style::Resize | sf::Style::Close);

        // Initialize the dashboard with full height of the window
        DashVisual dashVisual(200.0f, static_cast<float>(window.getSize().y));  // Use getSize().y for height
        dashVisual.loadFont("fonts/Roboto-Regular.ttf");

        NetworkMap networkMap(hosts);
        PortAnalysis portAnalysis(hosts);

        while (window.isOpen()) {
            sf::Event event{};
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                // Handle events based on the active view
                if (dashVisual.isActiveButton("Network Map")) {
                    networkMap.handleEvents(window, event);
                } else if (dashVisual.isActiveButton("Port Analysis")) {
                    portAnalysis.handleEvents(window, event);
                }
            }

            // Clear the window once per frame
            window.clear();

            // Draw based on the active view
            if (dashVisual.isActiveButton("Network Map")) {
                networkMap.draw(window);
            } else if (dashVisual.isActiveButton("Port Analysis")) {
                portAnalysis.draw(window);
            }

            // Draw the dashboard last to ensure it stays on top
            dashVisual.draw(window);

            // Display the rendered frame
            window.display();
        }

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}
