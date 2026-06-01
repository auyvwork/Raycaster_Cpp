cmake_minimum_required(VERSION 3.20)
project(game)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/dev/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3.1 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(game
        main.cpp
        map.h map.cpp
        player.h player.cpp
        renderer.h renderer.cpp
        SoundManager.cpp
)

target_link_libraries(game PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/dev/SFML-3.1.0/bin/sfml-graphics-3.dll"
        "C:/dev/SFML-3.1.0/bin/sfml-window-3.dll"
        "C:/dev/SFML-3.1.0/bin/sfml-system-3.dll"
        "C:/dev/SFML-3.1.0/bin/sfml-audio-3.dll"
        $<TARGET_FILE_DIR:game>
)
