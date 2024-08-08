cmake_minimum_required(VERSION 3.10)
project(nmap_project LANGUAGES CXX ASM)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(/usr/include)

# Add sources
set(SOURCES
        src/main.cpp
        src/NmapParser.cpp
        src/NetworkMap.cpp
        src/EventHandler.cpp
        src/FileSelector.c)

# Add sources with headers
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)

# Add executable target
add_executable(nmap_project ${SOURCES})

# Find and link SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(nmap_project sfml-graphics sfml-window sfml-system)
target_link_libraries(nmap_project tinyxml2)


# Find and link GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(nmap_project ${GTK3_LIBRARIES})
