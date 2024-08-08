cmake_minimum_required(VERSION 3.10)
project(nmap_project LANGUAGES CXX ASM)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add sources
set(SOURCES
        src/main.cpp
        src/NmapParser.cpp
        src/NetworkMap.cpp
        src/FileSelector.c
)

# Add sources with headers
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)

# Add executable target
add_executable(nmap_project ${SOURCES})

# Find and link SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(nmap_project sfml-graphics sfml-window sfml-system)


# Find TinyXML2 library and include paths
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (EXISTS "/usr/lib/x86_64-linux-gnu/libtinyxml2.so")
        # Debian-based systems
        set(TINYXML2_INCLUDE_DIR /usr/include)
        set(TINYXML2_LIBRARY /usr/lib/x86_64-linux-gnu/libtinyxml2.so)
    elseif (EXISTS "/usr/lib64/libtinyxml2.so")
        # Fedora-based systems
        set(TINYXML2_INCLUDE_DIR /usr/include)
        set(TINYXML2_LIBRARY /usr/lib64/libtinyxml2.so)
    else()
        message(FATAL_ERROR "TinyXML2 library not found.")
    endif()
else()
    message(FATAL_ERROR "Unsupported operating system.")
endif()

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

# Find and link GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(nmap_project ${GTK3_LIBRARIES})
