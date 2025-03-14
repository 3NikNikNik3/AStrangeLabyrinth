#include "Drawer.hpp"

#include <algorithm>
#include <thread>

#include <filesystem>
#include <fstream>

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

                float b1 = mod_pi((line.a - pos).ang());
                float b2 = mod_pi((line.b - pos).ang());
                if (b1 > b2)
                    std::swap(b1, b2);

                if (a2 - a1 > PI && b2 - b1 > PI) {
                    return check_aline_on_aline(a2, a1 + 2 * PI, b2, b1 + 2 * PI);
                }
                else if (a2 - a1 > PI)
                    return check_aline_on_aline(a2, PI, b1, b2) || check_aline_on_aline(-PI, a1, b1, b2);
                else if (b2 - b1 > PI)
                    return check_aline_on_aline(a1, a2, b2, PI) || check_aline_on_aline(a1, a2, -PI, b1);
                return check_aline_on_aline(a1, a2, b1, b2);
            }

            bool check_see_on_line(float a, std::pair<float, float> ages) {
                if (ages.second - ages.first > PI) {
                    return ages.second <= a || a <= ages.first;
                }
                return ages.first <= a && a <= ages.second;
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
            Room::Room(Tiles::Tile* tile, Vector pos, float a_see, float how_see, uchar from, uchar S) : tile(tile), pos(pos), a_see(a_see), how_see(how_see) {
                for (int i = 0; i < tile->boards.size(); ++i) {
                    if (check_line_on_see(tile->boards[i].first, pos, a_see, how_see)) {
                        if (tile->boards[i].second > 3 )
                            boards.push_back(new Board(tile->boards[i].first, pos, tile->boards[i].second));
                        else {
                            if (tile->boards[i].second == from || S >= 20) ;
                                //boards.push_back(new Portal(tile->boards[i].first, tile->boards[i].second, nullptr, pos));
                            else
                                boards.push_back(new Portal(tile->boards[i].first, tile->boards[i].second, tile->go[tile->boards[i].second], pos, a_see, how_see, tile->boards[i].second, S + 1));
                        }
                    }
                }
            }

            Room::~Room() {
                for (int i = 0; i < boards.size(); ++i) {
                    delete boards[i];
                }
            }

            Vector get_pos(Vector old_pos, uchar type_portal) {
                if (type_portal == 0)
                    old_pos.y += 3;
                else if (type_portal == 1)
                    old_pos.x -= 3;
                else if (type_portal == 2)
                    old_pos.y -= 3;
                else
                    old_pos.x += 3;

                return old_pos;
            }

            std::vector<Board*> Room::get_boards() {
                return boards;
            }

            // Portal
            Portal::Portal(Line line, uchar type, Tiles::Tile* tile, Vector pos, float a_see, float how_see, uchar from, uchar S) : Board(line, pos, type) {
                room = new Room(tile, get_pos(pos, type), a_see, how_see, (2 + from) % 4, S);
            }

            Portal::Portal(Line line, uchar type, Room *room, Vector pos) : Board(line, pos, type), room(room) {}

            Portal::~Portal() {
                delete room;
            }
		}

		// Setting
		uchar Setting::h_x = 1, Setting::scale_x = 1, Setting::fps = 60;

        unsigned short Setting::mouse_speed = 300;

        bool Setting::use_mouse = false;

        void Setting::load() {
            if (std::filesystem::exists("data/setting.data")) {
                std::ifstream file("data/setting.data");

                h_x = file.get();
                scale_x = file.get();
                fps = file.get();
                mouse_speed = file.get() * 256 + file.get();

                use_mouse = file.get();

                file.close();
            }
        }

        void Setting::save() {
            if (!std::filesystem::exists("data"))
                std::filesystem::create_directories("data");

            std::ofstream file("data/setting.data");

            file << h_x << scale_x << fps;
            file << (char)(mouse_speed / 256) << (char)(mouse_speed % 256);
            file << (char)use_mouse;

            file.close();
        }

		void calculate_lines(std::pair<float, char> *Ss_ans, int s, int e, Ray::Room* root_room, Vector pos, float a, float how_see, int n) {
            for (int i = s; i < e; ++i)
                Ss_ans[i] = calculate_line(root_room, pos, Ray::mod_pi(a - how_see / 2 + how_see / n * i));
		}

		std::pair<float, char> calculate_line(Ray::Room* root_room, Vector pos, float a) {
            int from_ = -1;
            Ray::Board *ans = nullptr;
            float ans_S = -1;

            Vector norm_v = Vector(cos(a), sin(a));

            while (ans == nullptr || ans->type < 4) {
                std::vector<Ray::Board*> arr = root_room->get_boards();
                Ray::Board *local_ans = nullptr;
                float loc_S = 20;

                for (int i = 0; i < arr.size(); ++i) {
                    if ((ans == nullptr || arr[i]->type != (ans->type + 2) % 4) && Ray::check_see_on_line(a, arr[i]->angs)) { // "delete" portal to previous room
                        float now_S = arr[i]->S / ((norm_v * arr[i]->perp) / (arr[i]->perp.len() * norm_v.len()));
                        if (now_S < 0) {
                            now_S *= -1;
                        }

                        if (now_S < loc_S) {
                            loc_S = now_S;
                            local_ans = arr[i];
                        }
                    }
                }

                ans = local_ans;
                ans_S = loc_S;
                if (local_ans == nullptr) {
                    break;
                } else if (ans->type < 4) {
                    root_room = ((Ray::Portal*)ans)->room;
                }
            }

            if (ans != nullptr)
                return {ans_S, ans->type};
            return {10, -1};
		}

		uchar now_col(uchar old, float dist) {
            return std::min(std::max((int)(old * (20 - dist) / 20), 0), 255);
		}

		void draw_line(std::pair<float, char> data, int x, sf::RenderWindow& window, std::pair<sf::Color, sf::Color> textures, int h_x) {
            if (data.second != -1) {
                sf::Color col_now;
                if (data.second == 4)
                    col_now = textures.first;
                else
                    col_now = textures.second;

                float size_see = 0.5 / data.first;

                sf::RectangleShape rect({h_x, window.getSize().y * size_see});
                rect.setFillColor({now_col(col_now.r, data.first), now_col(col_now.g, data.first), now_col(col_now.b, data.first)});
                rect.setPosition({x, window.getSize().y / 2 * (1 - size_see)});

                window.draw(rect);
            }
		}

		void draw_see(Tiles::Tile* tile, Vector pos, float a_see, float how_see, int n, int x, int y, int h_x, sf::RenderWindow& window) {
            Ray::Room root_room = Ray::Room(tile, pos, a_see, how_see, 255);

            x /= n;

            std::pair<float, char> *Ss = new std::pair<float, char>[n];

            unsigned int count_th = /*std::max(std::thread::hardware_concurrency() - 1, (unsigned int)0)*/0;
            unsigned int go = n / (count_th + 1);
            std::thread* threads[count_th];

            for (int i = 0; i < count_th; ++i)
                threads[i] = new std::thread(calculate_lines, Ss, i * go, (i + 1) * go, &root_room, pos, a_see, how_see, n);

            calculate_lines(Ss, count_th * go, n, &root_room, pos, a_see, how_see, n);

            for (int i = 0; i < count_th; ++i) {
                threads[i]->join();
                delete threads[i];
            }

            for (int i = 0; i < n; ++i) {
                draw_line(Ss[i], x * i, window, {sf::Color(128, 128, 128), sf::Color(0, 128, 128)}, h_x);
            }

            delete[] Ss;
		}

		bool ok(Vector pos, Tiles::Tile* tile) {
            float A, B, C, S;
            Vector perp(0, 0), line(0, 0);

            for (int i = 0; i < tile->boards.size(); ++i)
                if (tile->boards[i].second > 3) {
                    line = tile->boards[i].first.b - tile->boards[i].first.a;
                    //perp = (tile->boards[i].first.b - tile->boards[i].first.a).rot90();
                    //if ((perp * (pos - tile->boards[i].first.a) < 0) != (perp * (pos - tile->boards[i].first.b) < 0)) {
                    if (((tile->boards[i].first.a - pos) * (tile->boards[i].first.b - tile->boards[i].first.a) > 0) != ((tile->boards[i].first.b - pos) * (tile->boards[i].first.b - tile->boards[i].first.a) > 0)) {
                        A = tile->boards[i].first.b.y - tile->boards[i].first.a.y;
                        B = tile->boards[i].first.a.x - tile->boards[i].first.b.x;
                        C = tile->boards[i].first.b.x * tile->boards[i].first.a.y - tile->boards[i].first.a.x * tile->boards[i].first.b.y;

                        S = (A * pos.x + B * pos.y + C) / sqrt(A * A + B * B);
                        if (S < 0)
                            S *= -1;
                    } else
                        S = std::min((pos - tile->boards[i].first.a).len(), (pos - tile->boards[i].first.b).len());

                    if (S < R_PLAYER) {
                        return false;
                    }
                }
            return true;
		}

		void move_player(Vector &pos, Tiles::Tile*& tile, Vector shift) {
            if (ok(pos + shift, tile))
                pos += shift;
            else if (ok(pos + Vector(shift.x, 0), tile))
                pos.x += shift.x;
            else if (ok(pos + Vector(0, shift.y), tile))
                pos.y += shift.y;

            if (pos.x < 0 && tile->go[3] != nullptr) {
                tile = tile->go[3];
                pos = Ray::get_pos(pos, 3);
            }
            else if (pos.x > 3 && tile->go[1] != nullptr) {
                tile = tile->go[1];
                pos = Ray::get_pos(pos, 1);
            }
            else if (pos.y < 0 && tile->go[0] != nullptr) {
                tile = tile->go[0];
                pos = Ray::get_pos(pos, 0);
            }
            else if (pos.y > 3 && tile->go[2] != nullptr) {
                tile = tile->go[2];
                pos = Ray::get_pos(pos, 2);
            }
		}
	}
}

