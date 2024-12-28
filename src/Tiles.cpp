#include "Tiles.hpp"

using namespace AStrangeLabyrinth::Math;

namespace AStrangeLabyrinth {
	namespace Tiles {
		Tile::Tile() {
            go = new Tile*[4];
        }

        Tile::~Tile() {
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
                    boards.push_back({Line({0, 1}, {2, 2}), 5});
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
                    boards.push_back({Line({1, 2}, {2, 2}), 2});
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
	}
}

