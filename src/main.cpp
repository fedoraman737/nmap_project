#include <SFML/Graphics.hpp>
#include "NmapParser.h"
#include "NetworkMap.h"

int main() {
    NmapParser parser;
    std::vector<Host> hosts = parser.parseNmapXML("/home/fedora12/CLionProjects/nmap_project/output/nmap_output.xml");

    NetworkMap networkMap(hosts);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Network Visualizer");

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

    return 0;
}
