#pragma once

#include "Math.hpp"
#include <fstream>

#include <vector>

namespace AStrangeLabyrinth {
	namespace Tiles {
		struct Tile {
            Tile** go;

            bool end, is_delete = false;

            std::vector<std::pair<Math::Line, uchar>> boards;

            Tile(bool end=false);

            ~Tile();

            void update_boards();
		};

		namespace Generater {
            struct Settings {
                // 0 - 3/2, 1 - 2/1, 2 - 1/0, else - 0
                uchar depth_forks[3] = {2, 2, 2};

                uchar count_start_forks = 4; // [1; 4]

                Settings() = default;

                Settings(uchar depth_forks[3], uchar count_start_forks);

                Settings(uchar start_depth, uchar midle_depth, uchar end_depth, uchar count_start_forks);

                friend std::ifstream& operator>>(std::ifstream& in, Settings& setting);

                friend std::ofstream& operator<<(std::ofstream& out, Settings& setting);

                static bool ok(std::string name);
            };

            Tile* generate_rooms(Settings settings, Tile *p = nullptr, uchar from_ = -1, uchar S = 0);

            Tile* generate(Settings settings);
		}
	}
}

