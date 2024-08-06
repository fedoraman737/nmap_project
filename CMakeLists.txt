cmake_minimum_required(VERSION 3.10)
project(nmap_project LANGUAGES CXX ASM)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add sources
# Add sources
set(SOURCES
        src/main.cpp
        src/NmapParser.cpp
        src/NetworkMap.cpp)

# Add sources with headers
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)

# Add executable target
add_executable(nmap_project ${SOURCES})

# Find and link SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(nmap_project sfml-graphics sfml-window sfml-system)

# Manually specify TinyXML2 paths
set(TINYXML2_INCLUDE_DIR /usr/include)
set(TINYXML2_LIBRARY /usr/lib64/libtinyxml2.so)

include_directories(${TINYXML2_INCLUDE_DIR})
target_link_libraries(nmap_project ${TINYXML2_LIBRARY})

# Have cmake find nasm
find_program(NASM nasm)

if (NASM)
    message(STATUS "Found NASM : ${NASM}")
else()
    message(FATAL_ERROR "Cannot find NASM")
endif()

# Define the custom command for the NASM assembly
add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/src/panning.o
        DEPENDS ${CMAKE_SOURCE_DIR}/src/panning.asm
        COMMAND ${NASM} -f elf64 -o panning.o ${CMAKE_SOURCE_DIR}/src/panning.asm
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
        COMMENT "Generating object file from assembly source"
)

# Add the generated object file to the target sources
target_sources(nmap_project PUBLIC ${CMAKE_SOURCE_DIR}/src/panning.o)