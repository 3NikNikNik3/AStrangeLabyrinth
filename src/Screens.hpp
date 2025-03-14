#pragma once

#include <vector>

#include "gui.hpp"
#include "Tiles.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace AStrangeLabyrinth {
	namespace Screens {
        class ScreenWithGUI {
        protected:
            std::vector<GUI::Element*> arr;
            int now_select = 0;

            void draw(sf::RenderWindow& window);

            void click(sf::RenderWindow& window, int x, int y);

            void keydown(sf::Keyboard::Scancode key);
		};

        class ScreenSetting : public ScreenWithGUI {
            GUI::Button back_but;

            GUI::Number h_x, scale_x, mouse_speed, fps;

            GUI::Check use_mouse;

        public:
            ScreenSetting();

            bool go(sf::RenderWindow& window);
        };

        class ScreenPause : public ScreenWithGUI {
            GUI::Button play, setting_but, exit, save;

            GUI::Check use_mouse;

            ScreenSetting setting_screen;

        public:
            ScreenPause();

            uchar go(sf::RenderWindow& window, unsigned int seed, Tiles::Generater::Settings& setting);
        };

		class ScreenDraw {
            ScreenPause pause;

		public:
            const float SPEED = 1;

            bool go(Tiles::Tile* tile, sf::RenderWindow& window, unsigned int seed, Tiles::Generater::Settings& setting);
		};

		class ScreenPlaySetting : public ScreenWithGUI {
            ScreenDraw main_loop;

            Tiles::Generater::Settings setting;
            std::string name;

            GUI::Button back_but, play, load_but, save_but;

            GUI::Number start_tile, count_forks[3];

            void save();

            void load(std::string from);

            unsigned int load_with_seed(std::string from);

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

            ScreenSetting setting_screen;

        public:
            ScreenStart();

            void go(sf::RenderWindow& window);
        };
	}
}

