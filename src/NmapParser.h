#ifndef NMAP_PARSER_H
#define NMAP_PARSER_H

#include "tinyxml2.h"
#include <vector>
#include <string>

struct Host {
    std::string ip;
    std::vector<std::string> openPorts;
};

class NmapParser {
public:
    std::vector<Host> parseNmapXML(const std::string& filename);
};

#endif // NMAP_PARSER_H
