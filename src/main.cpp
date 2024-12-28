#include "Screens.hpp"
#include "Drawer.hpp"

using namespace AStrangeLabyrinth;

int main() {
    Tiles::Tile q;
    Drawer::draw_see(&q, {1.5f, 1.5f}, 0, Math::PI / 2, 1000);

	return 0;
}
