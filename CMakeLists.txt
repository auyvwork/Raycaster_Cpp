cmake_minimum_required(VERSION 3.20)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "/usr/local/Cellar/sfml/3.0.2/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(game MACOSX_BUNDLE
        main.cpp
        map.h
        map.cpp
        player.h
        player.cpp
        renderer.h
        renderer.cpp)

target_link_libraries(game SFML::Graphics SFML::Window SFML::System)

set_target_properties(game PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Raycaster 3D"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.auyv.raycaster"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
)

if(APPLE)
    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)
    target_link_libraries(game ${COCOA} ${IOKIT})
endif()