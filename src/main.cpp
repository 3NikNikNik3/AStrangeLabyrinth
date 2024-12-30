#include "Screens.hpp"
#include "Drawer.hpp"

using namespace AStrangeLabyrinth;

int main() {
    Tiles::Tile q, w;
    w.go[0] = &q;
    q.go[2] = &w;
    q.update_boards();
    w.update_boards();

    Drawer::draw_see(&q, {1.5f, 1.5f}, -Math::PI / 2, Math::PI / 2, 1000);

	return 0;
}
