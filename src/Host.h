#ifndef HOST_H
#define HOST_H

#include <string>
#include <vector>

struct Host {
    std::string ip;
    std::vector<std::string> openPorts;
};

#endif // HOST_H
