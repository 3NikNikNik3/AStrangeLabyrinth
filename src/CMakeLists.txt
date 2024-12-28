cmake_minimum_required(VERSION 3.28)

project(src)

add_library(src Drawer.hpp
		Drawer.cpp
		Screens.hpp
		Screens.cpp
		Tiles.hpp
		Tiles.cpp
		gui.hpp
		gui.cpp)
