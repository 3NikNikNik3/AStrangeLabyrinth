#include "Screens.hpp"
#include "Drawer.hpp"

using namespace AStrangeLabyrinth;
using namespace Screens;

int main() {
    Drawer::Setting::load();

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

    /*Tiles::Tile *room = new Tiles::Tile();

    room->go[0] = room;
    room->go[2] = room;

    room->update_boards();*/

    sf::RenderWindow window(sf::VideoMode({800, 600}), "A strange labyrinth", sf::Style::Default);

    window.setFramerateLimit(30);

    ScreenStart main_screen;

    main_screen.go(window);

    window.close();

    Drawer::Setting::save();

    return 0;
}
