#include "Screens.hpp"
#include "Drawer.hpp"

using namespace AStrangeLabyrinth;

int main() {
    Tiles::Tile q, w;
    w.go[0] = &q;
    q.go[2] = &w;
    q.update_boards();
    w.update_boards();

    sf::RenderWindow window(sf::VideoMode({800, 600}), "A strange labyrinth", sf::Style::Default);

    window.setFramerateLimit(30);

    Drawer::main_draw(&q, window);

	return 0;
}
