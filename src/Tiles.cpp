#include "Tiles.hpp"

using namespace AStrangeLabyrinth::Math;

namespace AStrangeLabyrinth {
	namespace Tiles {
		Tile::Tile(bool end) : end(end) {
            go = new Tile*[4];
        }

        Tile::~Tile() {
            is_delete = true;
            for (int i = 0; i < 4; ++i) {
                if (go[i] != nullptr && !go[i]->is_delete) {
                    delete go[i];
                }
            }
            delete[] go;
        }

        void Tile::update_boards() {
            boards.clear();

            if (end) {
                if (go[0] != nullptr) {
                    boards.push_back({Line({1, 0}, {1, 2}), 4});
                    boards.push_back({Line({1, 2}, {2, 2}), 5});
                    boards.push_back({Line({2, 0}, {2, 2}), 4});
                } else if (go[1] != nullptr) {
                    boards.push_back({Line({3, 1}, {1, 1}), 4});
                    boards.push_back({Line({1, 1}, {1, 2}), 5});
                    boards.push_back({Line({1, 2}, {3, 2}), 4});
                } else if (go[2] != nullptr) {
                    boards.push_back({Line({1, 1}, {1, 3}), 4});
                    boards.push_back({Line({1, 1}, {2, 1}), 5});
                    boards.push_back({Line({2, 1}, {2, 3}), 4});
                } else {
                    boards.push_back({Line({0, 1}, {2, 1}), 4});
                    boards.push_back({Line({2, 1}, {2, 2}), 5});
                    boards.push_back({Line({2, 2}, {0, 2}), 4});
                }
            } else {
                if (go[0] == nullptr) {
                    boards.push_back({Line({1, 1}, {2, 1}), 4});
                } else {
                    boards.push_back({Line({1, 0}, {1, 1}), 4});
                    boards.push_back({Line({1, 0}, {2, 0}), 0});
                    boards.push_back({Line({2, 0}, {2, 1}), 4});
                }

                if (go[1] == nullptr) {
                    boards.push_back({Line({2, 1}, {2, 2}), 4});
                } else {
                    boards.push_back({Line({2, 1}, {3, 1}), 4});
                    boards.push_back({Line({3, 1}, {3, 2}), 1});
                    boards.push_back({Line({3, 2}, {2, 2}), 4});
                }

                if (go[2] == nullptr) {
                    boards.push_back({Line({1, 2}, {2, 2}), 4});
                } else {
                    boards.push_back({Line({1, 3}, {1, 2}), 4});
                    boards.push_back({Line({1, 3}, {3, 3}), 2});
                    boards.push_back({Line({2, 2}, {2, 3}), 4});
                }

                if (go[3] == nullptr) {
                    boards.push_back({Line({1, 1}, {1, 2}), 4});
                } else {
                    boards.push_back({Line({1, 1}, {0, 1}), 4});
                    boards.push_back({Line({0, 1}, {0, 2}), 3});
                    boards.push_back({Line({0, 2}, {1, 2}), 4});
                }
            }
        }

        namespace Generater {
            // Settings
            Settings::Settings(uchar depth_forks[3], uchar count_start_forks, uchar end_event[4]) :
                depth_forks({depth_forks[0], depth_forks[1], depth_forks[2]}),
                count_start_forks(count_start_forks),
                end_event({end_event[0], end_event[1], end_event[2], end_event[3]}),
                sum_end_event(end_event[0] + end_event[1] + end_event[2] + end_event[3]) {}

            Settings::Settings(uchar start_depth, uchar midle_depth, uchar end_depth, uchar count_start_forks,
                               uchar simple_end, uchar end_corridor, uchar end_rotation, uchar end_fake) :
                depth_forks({start_depth, midle_depth, end_depth}),
                count_start_forks(count_start_forks),
                end_event({simple_end, end_corridor, end_rotation, end_fake}),
                sum_end_event(simple_end + end_corridor + end_rotation + end_fake) {}

            std::ifstream& operator>>(std::ifstream& in, Settings& setting) {
                for (int i = 0; i < 3; ++i)
                    setting.depth_forks[i] = in.get();

                setting.sum_end_event = 0;
                for (int i = 0; i < 4; ++i) {
                    setting.end_event[i] = in.get();
                    setting.sum_end_event += setting.end_event[i];
                }

                setting.count_start_forks = in.get();

                return in;
            }

            std::ofstream& operator<<(std::ofstream& out, Settings& setting) {
                for (int i = 0; i < 3; ++i)
                    out << setting.depth_forks[i];

                for (int i = 0; i < 4; ++i)
                    out << setting.end_event[i];

                out << setting.count_start_forks;

                return out;
            }

            bool Settings::ok(std::string name) {
                std::ifstream in(name);

                int ok_i = 8;
                if (name[name.size() - 5] == 't')
                    ok_i = 12;

                int i = 0;
                for (; i < ok_i && !in.eof(); ++i) {
                    in.get();
                }

                in.close();

                return i == ok_i;
            }

            // Generate
            std::vector<Tile*> ends;

            Tile* generate_rooms(Settings settings, Tile *p, uchar from_, uchar S) {
                Tile* res = new Tile();

                if (S == 0) {
                    // Start room
                    int none = 4 - settings.count_start_forks;
                    for (int i = 0; i < 4; ++i) {
                        if (none != 0) {
                            if (4 - i == none || rand() % 2 == 0) {
                                --none;
                                res->go[i] = nullptr;
                            }
                            else
                                res->go[i] = generate_rooms(settings, res, (2 + i) % 4, S + 1);
                        } else
                            res->go[i] = generate_rooms(settings, res, (2 + i) % 4, S + 1);
                    }
                } else {
                    // Else room
                    res->go[from_] = p;

                    uchar add; // how count to add
                    if (settings.depth_forks[0] >= S) {
                        if (rand() % 2 == 0)
                            add = 3;
                        else
                            add = 2;
                    } else if (settings.depth_forks[0] + settings.depth_forks[1] >= S) {
                        if (rand() % 2 == 0)
                            add = 2;
                        else
                            add = 1;
                    } else if (settings.depth_forks[0] + settings.depth_forks[1] + settings.depth_forks[2] >= S) {
                        if (rand() % 2 == 0)
                            add = 1;
                        else {
                            add = 0;
                            ends.push_back(res);
                        }
                    } else {
                        add = 0;
                        ends.push_back(res);
                    }

                    // generate
                    /*for (int i = 0; i < 4; ++i) {
                        if (from_ != i) {
                            if (add != 0 && (add == 3 - i || rand() % 2 == 0)) {
                                res->go[i] = generate_rooms(settings, res, (2 + i) % 4, S + 1);
                                --add;
                            } else
                                res->go[i] = nullptr;
                        }
                    }*/
                    for (int i = 0; i < 4; ++i)
                        if (i != from_)
                            res->go[i] = nullptr;

                    for (; add != 0; --add) {
                        int where = rand() % add; // id res->go on nullptr
                        int j = 0; // real id res->go
                        for (; where || res->go[j] != nullptr; ++j)
                            if (res->go[j] == nullptr)
                                --where;

                        res->go[j] = generate_rooms(settings, res, (2 + j) % 4, S + 1);
                    }
                }

                res->update_boards();
                return res;
            }

            std::pair<Tile*, unsigned int> generate(Settings settings) {
                // To delete the information of time from seed
                unsigned int seed = time(0) / 10000 * 10000 + (rand() % 1000) * 10 + time(0) % 10;

                return {generate(settings, seed), seed};
            }

            Tile* generate(Settings settings, unsigned int seed) {
                srand(seed);

                Tile* ans = generate_rooms(settings);

                // chose end room
                int who = rand() % ends.size();

                ends[who]->end = true;
                ends[who]->update_boards();

                ends.erase(ends.begin() + who);

                // chose end event room
                std::vector<bool> used(ends.size(), true);
                for (int i = 0; i < ends.size(); ++i) {
                    if (used[i]) {
                        // chose event
                        int what = rand() % settings.sum_end_event;
                        for (int i = 0; i < 4; ++i) {
                            if (what < settings.end_event[i]) {
                                what = i;
                                break;
                            }
                            what -= settings.end_event[i];
                        }

                        // set event
                        uchar come_where;
                        for (int j = 0; j < 4; ++j)
                            if (ends[i]->go[j] != nullptr) {
                                come_where = j;
                                break;
                            }

                        Tile *tiles[4];
                        Tile *in;
                        int who_sec, who_old;

                        switch (what) {
                            case 1: // endless corridor
                                ends[i]->go[(come_where + 1) % 4] = ends[i];
                                ends[i]->go[(come_where + 3) % 4] = ends[i];

                                ends[i]->update_boards();
                                break;

                            case 2: // endless rotation
                                for (int i = 0; i < 4; ++i)
                                    tiles[i] = new Tile();
                                for (int i = 0; i < 4; ++i) {
                                    tiles[i]->go[i] = tiles[(i + 1) % 4];
                                    tiles[(i + 1) % 4]->go[(i + 2) % 4] = tiles[i];

                                    tiles[i]->go[(i + 2) % 4] = tiles[i]->go[(i + 3) % 4] = nullptr;
                                }

                                in = new Tile();
                                in->go[come_where] = ends[i];
                                ends[i]->go[(come_where + 2) % 4] = in;

                                tiles[come_where]->go[(come_where + 1) % 4] = in;
                                in->go[(come_where + 3) % 4] = tiles[come_where];

                                in->go[(come_where + 1) % 4] = in->go[(come_where + 2) % 4] = nullptr;

                                for (int i = 0; i < 4; ++i)
                                    tiles[i]->update_boards();

                                in->update_boards();
                                ends[i]->update_boards();

                                break;

                            case 3: // fake end
                                //ends[i]->boards[5].second = 6;
                                //ends[i]->update_boards();

                                switch (come_where) {
                                    case 0:
                                        ends[i]->boards[4].second = 6;
                                        break;
                                    case 1:
                                        ends[i]->boards[4].second = 6;
                                        break;
                                    case 2:
                                        ends[i]->boards[0].second = 6;
                                        break;
                                    case 3:
                                        ends[i]->boards[1].second = 6;
                                        break;
                                }

                                break;
                        }
                    }
                }

                ends.clear();

                return ans;
            }
        }
	}
}

