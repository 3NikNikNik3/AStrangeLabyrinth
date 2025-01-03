#include "Drawer.hpp"

#include <algorithm>
#include <iostream>

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
            Room::Room(Tiles::Tile* tile, Vector pos, float a_see, float how_see) : tile(tile), pos(pos), a_see(a_see), how_see(how_see) {}

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
                if (boards.size() == 0) {
                    for (int i = 0; i < tile->boards.size(); ++i)
                        if (check_line_on_see(tile->boards[i].first, pos, a_see, how_see)) {
                            if (tile->boards[i].second > 3)
                                boards.push_back(new Board(tile->boards[i].first, pos, tile->boards[i].second));
                            else
                                boards.push_back(new Portal(tile->boards[i].first, tile->boards[i].second, tile->go[tile->boards[i].second], pos, a_see, how_see));
                        }
                }
                return boards;
            }

            // Portal
            Portal::Portal(Line line, uchar type, Tiles::Tile* tile, Vector pos, float a_see, float how_see) : Board(line, pos, type) {
                room = new Room(tile, get_pos(pos, type), a_see, how_see);
            }

            Portal::~Portal() {
                delete room;
            }
		}

		void draw_line(Ray::Room* root_room, Vector pos, float a, int x, sf::RenderWindow& window, std::pair<sf::Texture&, sf::Texture&> textures) {
            // What draw

            int from_ = -1;
            Ray::Board *ans = nullptr;
            float ans_S = -1;

            Vector norm_v = Vector(cos(a), sin(a));

            while (ans == nullptr || ans->type < 4) {
                std::vector<Ray::Board*> arr = root_room->get_boards();
                Ray::Board *local_ans = nullptr;
                float loc_S = 50;

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

            // Draw
            if (ans != nullptr) {
                sf::Texture tx_now;
                if (ans->type == 4)
                    tx_now = textures.first;
                else
                    tx_now = textures.second;

                float size_see = 0.5 / ans_S;

                sf::Sprite spr(tx_now);
                spr.setPosition({x, window.getSize().y / 2 * (1 - size_see)});
                spr.setScale({1.0f, size_see});

                window.draw(spr);
            }
		}

		void draw_see(Tiles::Tile* tile, Vector pos, float a_see, float how_see, int n, int x, int y, sf::RenderWindow& window) {
            Ray::Room root_room = Ray::Room(tile, pos, a_see, how_see);

            x /= n;
            sf::Texture texture(sf::Vector2u(x, y));

            std::vector<std::uint8_t> pix(x * y * 4);
            for (int i = 0; i < x * y; ++i) {
                pix[i * 4] = pix[i * 4 + 1] = pix[i * 4 + 2] = 128;
                pix[i * 4 + 3] = 255;
            }
            texture.update(pix.data());

            for (int i = 0; i < n; ++i) {
                draw_line(&root_room, pos, Ray::mod_pi(a_see - how_see / 2 + how_see / n * i), i * x, window, {texture, texture});
            }
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

		void main_draw(Tiles::Tile* tile, sf::RenderWindow& window) {
            float a = 0;

            const int h_x = 2, scale_x = 2;

            sf::View view(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
            window.setView(view);

            Vector pos = {1.5f, 1.5f};

            sf::Clock clock;

            while (window.isOpen()) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>())
                        window.close();
                    else if (const auto* resized = event->getIf<sf::Event::Resized>())
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x / scale_x, resized->size.y})));
                }

                float delta =  clock.restart().asSeconds();

                //a = Ray::mod_pi(a + 0.05);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
                    a = Ray::mod_pi(a + 2 * delta);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                    a = Ray::mod_pi(a - 2 * delta);

                Vector shift(0, 0);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    shift.x += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                    shift.y += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    shift.x -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                    shift.y -= 1;

                move_player(pos, tile, shift.rot(a).norm() * SPEED * delta);

                auto [w, h] = window.getSize();
                window.clear(sf::Color::White);

                draw_see(tile, pos, a, Math::PI / 2, w / scale_x / h_x, w / scale_x, h, window);

                window.display();
            }
		}
	}
}

