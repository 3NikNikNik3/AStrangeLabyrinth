#include "Screens.hpp"
#include "Drawer.hpp"

using namespace AStrangeLabyrinth;

int main() {
    /*Tiles::Tile *q = new Tiles::Tile(), *w = new Tiles::Tile(), *e = new Tiles::Tile(), *r = new Tiles::Tile();

    q->go[0] = w;
    w->go[2] = q;
    w->go[1] = e;
    e->go[3] = w;
    e->go[2] = r;
    r->go[0] = e;
    r->go[3] = q;
    q->go[1] = r;

    Tiles::Tile *a = new Tiles::Tile();
    q->go[1] = a;
    a->go[3] = q;

    a->go[1] = a;

    q->update_boards();
    w->update_boards();
    e->update_boards();
    r->update_boards();
    a->update_boards();*/

    Tiles::Tile *room = Tiles::Generater::generate_rooms(Tiles::Generater::Settings());

    sf::RenderWindow window(sf::VideoMode({800, 600}), "A strange labyrinth", sf::Style::Default);

    window.setFramerateLimit(30);

    Drawer::main_draw(room, window);

    delete room;

	return 0;
}
