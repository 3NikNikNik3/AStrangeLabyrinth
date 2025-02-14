#include "Tiles.hpp"

using namespace AStrangeLabyrinth::Math;

namespace AStrangeLabyrinth {
	namespace Tiles {
		Tile::Tile(bool end) : end(end) {
            go = new Tile*[4];
        }

        Tile::~Tile() {
            is_delete = true;
            for (int i = 0; i < 4; ++i)
                if (go[i] != nullptr && !go[i]->is_delete)
                    delete go[i];
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
            Settings::Settings(uchar depth_forks[3], uchar count_start_forks) : depth_forks({depth_forks[0], depth_forks[1], depth_forks[2]}), count_start_forks(count_start_forks) {}

            Settings::Settings(uchar start_depth, uchar midle_depth, uchar end_depth, uchar count_start_forks) : depth_forks({start_depth, midle_depth, end_depth}), count_start_forks(count_start_forks) {}

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

            Tile* generate(Settings settings) {
                Tile* ans = generate_rooms(settings);

                // chose end room
                int who = rand() % ends.size();

                ends[who]->end = true;
                ends[who]->update_boards();

                ends.clear();

                return ans;
            }
        }
	}
}

