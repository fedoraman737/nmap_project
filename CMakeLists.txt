cmake_minimum_required(VERSION 3.10)
project(NetworkVisualizer)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add sources
add_executable(NetworkVisualizer src/main.cpp src/NmapParser.cpp src/NetworkMap.cpp
        src/NmapParser.cpp
        src/NmapParser.h
        src/NetworkMap.cpp
        src/NetworkMap.h)

# Find and link SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(NetworkVisualizer sfml-graphics sfml-window sfml-system)

# Find and link TinyXML2
find_package(TinyXML2 REQUIRED)
target_link_libraries(NetworkVisualizer TinyXML2::TinyXML2)