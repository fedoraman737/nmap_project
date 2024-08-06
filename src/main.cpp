#include <SFML/Graphics.hpp>
#include "NmapParser.h"
#include "NetworkMap.h"
#include <iostream>
#include <exception>

int main() {
    try {
        NmapParser parser;
        std::vector<Host> hosts = parser.parseNmapXML("/home/fedora12/CLionProjects/nmap_project/output/nmap_output.xml");

        if (hosts.empty()) {
            std::cerr << "No hosts found in the Nmap XML file. Exiting..." << std::endl;
            return 1;
        }

        NetworkMap networkMap(hosts);

        sf::RenderWindow window(sf::VideoMode(800, 600), "Network Visualizer");

        if (!window.isOpen()) {
            std::cerr << "Failed to create SFML window. Exiting..." << std::endl;
            return 1;
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                networkMap.handleEvents(window, event);
            }

            window.clear();
            networkMap.draw(window);
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
