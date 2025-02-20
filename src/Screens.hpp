#pragma once

#include <vector>

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

		class ScreenWithGUI {
        protected:
            std::vector<GUI::Element*> arr;
            int now_select = 0;

            void draw(sf::RenderWindow& window);

            void click(sf::RenderWindow& window, int x, int y);

            void keydown(sf::Keyboard::Scancode key);
		};

        class ScreenStart : public ScreenWithGUI {
            GUI::Button but_exit, but_exit2;

        public:
            ScreenStart();

            void go(sf::RenderWindow& window);
        };
	}
}

