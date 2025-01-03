#pragma once

#include "Tiles.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace AStrangeLabyrinth::Math;

namespace AStrangeLabyrinth {
	namespace Drawer {
		namespace Ray {
            float mod_pi(float a);

            bool check_line_on_see(Line line, Vector pos, float a_see, float how_see);

            bool check_aline_on_aline(float a1, float a2, float b1, float b2);

            class Board {
                public:

                Line line;
                std::pair<float, float> angs;
                float S;
                Vector perp; // perpendicular

                uchar type; // 0-3 portal, 4 - norm, 5 - door

                Board(Line line, Vector pos, uchar type);
            };

            class Room {
                std::vector<Board*> boards;
                Vector pos;
                float a_see, how_see;
                Tiles::Tile* tile;

                public:

                Room(Tiles::Tile* tile, Vector pos, float a_see, float how_see);

                ~Room();

                std::vector<Board*> get_boards();
            };

            class Portal : public Board {
                public:

                Room* room;

                Portal(Line line, uchar type, Tiles::Tile* tile, Vector pos, float a_see, float how_see);

                ~Portal();
            };
		}

		void draw_line(Ray::Room* root_room, Vector pos, float a, int x, sf::RenderWindow& window, std::pair<sf::Texture&, sf::Texture&> textures);

        void draw_see(Tiles::Tile* tile, Vector pos, float a_see, float how_see, int n, int x, int y, sf::RenderWindow& window);

        void move_player(Vector &pos, Tiles::Tile*& tile, Vector shift);

        const float R_PLAYER = 0.05;

        bool ok(Vector pos, Tiles::Tile* tile);

        const float SPEED = 1;

        void main_draw(Tiles::Tile* root, sf::RenderWindow& window);
	}
}
