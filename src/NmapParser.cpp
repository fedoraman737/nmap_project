#include "NmapParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

std::vector<Host> NmapParser::parseNmapXML(const std::string& filename) {
    std::vector<Host> hosts;

    // Read the entire file into a string
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open Nmap XML file: " << filename << "\n";
        return hosts;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // Remove XML declaration and stylesheet processing instruction
    auto xmlDeclPos = content.find("<?xml");
    if (xmlDeclPos != std::string::npos) {
        auto xmlDeclEndPos = content.find("?>", xmlDeclPos);
        if (xmlDeclEndPos != std::string::npos) {
            content.erase(xmlDeclPos, xmlDeclEndPos - xmlDeclPos + 2);
        }
    }

    auto xmlStylesheetPos = content.find("<?xml-stylesheet");
    if (xmlStylesheetPos != std::string::npos) {
        auto xmlStylesheetEndPos = content.find("?>", xmlStylesheetPos);
        if (xmlStylesheetEndPos != std::string::npos) {
            content.erase(xmlStylesheetPos, xmlStylesheetEndPos - xmlStylesheetPos + 2);
        }
    }

    // Parse the modified content
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError eResult = doc.Parse(content.c_str());
    if (eResult != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to parse Nmap XML content. Error: " << eResult << "\n";
        std::cerr << "Error description: " << doc.ErrorStr() << "\n";
        return hosts;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element in XML file: " << filename << "\n";
        return hosts;
    }

    for (tinyxml2::XMLElement* hostElem = root->FirstChildElement("host"); hostElem; hostElem = hostElem->NextSiblingElement("host")) {
        Host host;
        tinyxml2::XMLElement* addressElem = hostElem->FirstChildElement("address");
        if (addressElem) {
            host.ip = addressElem->Attribute("addr");
        }
        for (tinyxml2::XMLElement* portElem = hostElem->FirstChildElement("ports")->FirstChildElement("port"); portElem; portElem = portElem->NextSiblingElement("port")) {
            host.openPorts.push_back(portElem->Attribute("portid"));
        }
        hosts.push_back(host);

        // Debug output
        std::cout << "Host IP: " << host.ip << "\n";
        for (const auto& port : host.openPorts) {
            std::cout << "  Open Port: " << port << "\n";
        }
    }

    return hosts;
}
