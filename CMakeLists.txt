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

if (build_static)
	set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

if (WIN32)
	add_executable(AStrangeLabyrinth WIN32 src/main.cpp)
	#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}" -static-libgcc -static-libstdc++")
else()
	add_executable(AStrangeLabyrinth src/main.cpp)
endif()

target_link_libraries(src SFML::Graphics)

target_link_libraries(AStrangeLabyrinth src)
