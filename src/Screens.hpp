#pragma once

#include "gui.hpp"
#include "Tiles.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace AStrangeLabyrinth {
	namespace Screens {
		class ScreenDraw {
		public:
            const float SPEED = 1;

            void go(Tiles::Tile* tile, sf::RenderWindow& window);
		};
	}
}

