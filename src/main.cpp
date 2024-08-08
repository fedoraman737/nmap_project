#include <SFML/Graphics.hpp>
#include "NmapParser.h"
#include "NetworkMap.h"
#include "FileSelector.h"
#include <iostream>
#include <exception>
#include <fstream>

int main() {
    try {
        std::cout << "Starting file selection..." << std::endl;
        // Call the file selection function
        select_nmap_file();

        std::cout << "File selection done. Reading the selected file name..." << std::endl;
        // Read the selected file name
        std::ifstream file("selected_file.txt");
        std::string nmapFilePath;
        if (!file || !(file >> nmapFilePath)) {
            std::cerr << "Failed to read the selected Nmap file. Exiting..." << std::endl;
            return 1;
        }

        // Clean up the selected_file.txt after reading the file path
        std::remove("selected_file.txt");

        std::cout << "Selected Nmap file: " << nmapFilePath << std::endl;

        NmapParser parser;
        std::vector<Host> hosts = parser.parseNmapXML(nmapFilePath);

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
