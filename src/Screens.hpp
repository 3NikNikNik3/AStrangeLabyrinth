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

            bool go(Tiles::Tile* tile, sf::RenderWindow& window);
		};

		class ScreenWithGUI {
        protected:
            std::vector<GUI::Element*> arr;
            int now_select = 0;

            void draw(sf::RenderWindow& window);

            void click(sf::RenderWindow& window, int x, int y);

            void keydown(sf::Keyboard::Scancode key);
		};

		class ScreenPlaySetting : public ScreenWithGUI {
            ScreenDraw main_loop;

            Tiles::Generater::Settings setting;
            std::string name;

            GUI::Button back_but, play, load_but, save_but;

            GUI::Number start_tile, count_forks[3];

            void save();

            void load(std::string from);

            std::string chose_file();

            #ifdef __linux__

            std::string path_me;

            #endif

        public:
            ScreenPlaySetting();

            bool go(sf::RenderWindow& window);
        };

        class ScreenStart : public ScreenWithGUI {
            GUI::Button play, play_setting, settings, exit;

            ScreenDraw main_loop;

            ScreenPlaySetting play_setting_screen;

        public:
            ScreenStart();

            void go(sf::RenderWindow& window);
        };
	}
}

