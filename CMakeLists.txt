cmake_minimum_required(VERSION 3.20)
project(game)

set(CMAKE_CXX_STANDARD 17)


find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)


add_executable(game main.cpp
        map.h
        map.cpp
        player.h
        player.cpp
        renderer.h
        renderer.cpp)


target_link_libraries(game SFML::Graphics SFML::Window SFML::System)


if(APPLE)
    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)
    target_link_libraries(game ${COCOA} ${IOKIT})
endif()