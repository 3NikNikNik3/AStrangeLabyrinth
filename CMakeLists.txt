cmake_minimum_required(VERSION 3.28)
project(AStrangeLabyrinth LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_subdirectory(src)

add_executable(AStrangeLabyrinth src/main.cpp)

target_link_libraries(src SFML::Graphics)
target_link_libraries(AStrangeLabyrinth src)
