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
            Board::Board(Line line, Vector pos, uchar type) : line(line), type(type), perp({0, 0}) {
                angs = {(line.a - pos).ang(), (line.b - pos).ang()};
                if (angs.first > angs.second)
                    std::swap(angs.first, angs.second);

                float A = line.b.y - line.a.y, B = line.a.x - line.b.x, C = line.b.x * line.a.y - line.a.x * line.b.y;

                S = (A * pos.x + B * pos.y + C) / sqrt(A * A + B * B);

                perp = (line.b - line.a).rot90();
            }

            // Room
            Room::Room(Tiles::Tile* tile, Vector pos, float a_see, float how_see) : tile(tile), pos(pos), a_see(a_see), how_see(how_see) {}

            Room::~Room() {
                for (int i = 0; i < boards.size(); ++i) {
                    delete boards[i];
                }
            }

            std::vector<Board*> Room::get_boards() {
                if (boards.size() == 0) {
                    for (int i = 0; i < tile->boards.size(); ++i)
                        if (check_line_on_see(tile->boards[i].first, pos, a_see, how_see)) {
                            boards.push_back(new Board(tile->boards[i].first, pos, tile->boards[i].second));
                        }
                }
                return boards;
            }

            // Portal
            Portal::Portal(Line line, uchar type, Tiles::Tile* tile, Vector pos, float a_see, float how_see) : Board(line, pos, type) {
                room = new Room(tile, pos, a_see, how_see);
            }

            Portal::~Portal() {
                delete room;
            }
		}

		void draw_line(Ray::Room* root_room, Vector pos, float a, int x) {
            // What draw

            int from_ = -1;
            Ray::Board *ans = nullptr;

            while (ans == nullptr || ans->type < 4) {
                std::vector<Ray::Board*> arr = root_room->get_boards();
                Ray::Board *local_ans = arr[0];
                float loc_S = 0;

                Vector norm_v = Vector(cos(a), sin(a));

                for (int i = 1; i < arr.size(); ++i) {
                    if (ans == nullptr || arr[i]->type != (ans->type + 2) % 4) { // "delete" portal to previous room
                        float now_S = arr[i]->S / ((norm_v * arr[i]->perp) / (arr[i]->perp.len() * norm_v.len()));

                        if (now_S < loc_S) {
                            loc_S = now_S;
                            local_ans = arr[i];
                        }
                    }
                }

                ans = local_ans;
                if (local_ans == nullptr) {
                    break;
                } else if (ans->type < 4) {
                    root_room = ((Ray::Portal*)ans)->room;
                }
            }

            // Draw
		}

		void draw_see(Tiles::Tile* tile, Vector pos, float a_see, float how_see, int n) {
            Ray::Room root_room = Ray::Room(tile, pos, a_see, how_see);

            for (int i = 0; i < n; ++i) {
                draw_line(&root_room, pos, a_see - how_see + how_see / n * i, i);
            }
		}
	}
}

