#include "NmapParser.h"

std::vector<Host> NmapParser::parseNmapXML(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    std::vector<Host> hosts;

    if (doc.LoadFile(filename.c_str()) == tinyxml2::XML_SUCCESS) {
        tinyxml2::XMLElement* root = doc.RootElement();
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
        }
    }
    return hosts;
}

