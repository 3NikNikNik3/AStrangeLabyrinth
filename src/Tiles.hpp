#pragma once

#include "Math.hpp"

#include <vector>

namespace AStrangeLabyrinth {
	namespace Tiles {
		struct Tile {
            Tile** go;

            bool end;

            std::vector<std::pair<Math::Line, uchar>> boards;

            Tile(bool end=false);

            ~Tile();

            void update_boards();
		};
	}
}

