cmake_minimum_required(VERSION 3.10)
project(nmap_project)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add sources
add_executable(nmap_project
        src/main.cpp
        src/NmapParser.cpp
        src/NmapParser.h
        src/NetworkMap.cpp
        src/NetworkMap.h)

# Find and link SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(nmap_project sfml-graphics sfml-window sfml-system)

# Manually specify TinyXML2 paths
set(TINYXML2_INCLUDE_DIR /usr/include)
set(TINYXML2_LIBRARY /usr/lib64/libtinyxml2.so)

include_directories(${TINYXML2_INCLUDE_DIR})
target_link_libraries(nmap_project ${TINYXML2_LIBRARY})
