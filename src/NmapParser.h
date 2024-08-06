#ifndef NMAP_PARSER_H
#define NMAP_PARSER_H

#include "Host.h"
#include <vector>
#include <string>

class NmapParser {
public:
    std::vector<Host> parseNmapXML(const std::string& filePath);
};

#endif // NMAP_PARSER_H
