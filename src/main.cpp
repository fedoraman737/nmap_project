#include <SFML/Graphics.hpp>
#include "NmapParser.h"
#include "EventHandler.h"
#include "NetworkMap.h"
#include <iostream>
#include <exception>

int main() {
    try {
        NmapParser parser;
        std::vector<Host> hosts = parser.parseNmapXML("/home/lightmodepidro/Desktop/nmap_project/nmap_output.xml");

        if (hosts.empty()) {
            std::cerr << "No hosts found in the Nmap XML file. Exiting..." << std::endl;
            return 1;
        }

        sf::RenderWindow window(sf::VideoMode(800, 600), "Network Visualizer");
        std::cout << hosts.size() << " Host\n";
        NetworkMap networkMap(hosts);
        EventHandler eventHandler(window, networkMap);

        if (!window.isOpen()) {
            std::cerr << "Failed to create SFML window. Exiting..." << std::endl;
            return 1;
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                eventHandler.handle(event);
            }

            window.clear();
            networkMap.draw(window);
            window.display();
        }

    } catch (const std::exception& error) {
        std::cerr << "An error occurred: " << error.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}
