#include "Drawer.hpp"

#include <algorithm>

namespace AStrangeLabyrinth {
	namespace Drawer {
		namespace Ray {
            float mod_pi(float a) {
                if (a < -PI)
                    a += 2 * PI;

                if (a > PI)
                    a -= 2 * PI;

                return a;
            }

            bool check_line_on_see(Line line, Vector pos, float a_see, float how_see) {
                float a1 = mod_pi(a_see - how_see / 2);
                float a2 = mod_pi(a_see + how_see / 2);
                if (a1 > a2)
                    std::swap(a1, a2);

                float b1 = (line.a - pos).ang();
                float b2 = (line.b - pos).ang();
                if (b1 > b2)
                    std::swap(b1, b2);

                if (a2 - a1 > PI && b2 - b1 > PI)
                    return check_aline_on_aline(a2, a1 + 2 * PI, b2, b1 + 2 * PI);
                else if (a2 - a1 > PI)
                    return check_aline_on_aline(a2, PI, b1, b2) || check_aline_on_aline(-PI, a1, b1, b2);
                else if (b2 - b1 > PI)
                    return check_aline_on_aline(a1, a2, b2, PI) || check_aline_on_aline(a1, a2, -PI, b1);
                return check_aline_on_aline(a1, a2, b1, b2);
            }

            bool check_aline_on_aline(float a1, float a2, float b1, float b2) {
                return !(a2 < b1 || b2 < a1);
            }

            // Board
            Board::Board(Line line, std::pair<float, float> angs, float S, uchar type) : line(line), angs(angs), S(S), type(type) {}

            // Room
            Room::Room(Tiles::Tile* tile, Vector pos, float a_see, float how_see) : tile(tile), pos(pos), a_see(a_see), how_see(how_see) {}

            Room::~Room() {
                for (int i = 0; i < boards.size(); ++i) {
                    delete boards[i];
                }
            }

            std::vector<Board*> Room::get_boards() {
                if (boards.size() == 0) {

                }
                return boards;
            }

            // Portal
            Portal::Portal(Line line, std::pair<float, float> angs, float S, uchar type, Tiles::Tile* tile, Vector pos, float a_see, float how_see) : Board(line, angs, S, type) {
                room = new Room(tile, pos, a_see, how_see);
            }

            Portal::~Portal() {
                delete room;
            }
		}

		void draw_line(Ray::Room* root_room, Vector pos, float a, int x) {
            //!
            root_room->get_boards();
		}

		void draw_see(Tiles::Tile* tile, Vector pos, float a_see, float how_see, int n) {
            Ray::Room root_room = Ray::Room(tile, pos, a_see, how_see);

            for (int i = 0; i < n; ++i) {
                draw_line(&root_room, pos, a_see - how_see + how_see / n * i, i);
            }
		}
	}
}

