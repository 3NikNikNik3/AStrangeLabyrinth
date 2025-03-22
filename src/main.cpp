#include "Screens.hpp"
#include "Drawer.hpp"

#include <filesystem>
#include <fstream>

#include <iostream>

using namespace AStrangeLabyrinth;
using namespace Screens;

int main(int argc, char* argv[]) {
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

    window.setFramerateLimit(Drawer::Setting::fps);

    if (argc == 1) {
        // Norm game

        ScreenStart main_screen;

        main_screen.go(window);
    } else if (argc == 2) {
        std::cerr << "\033[31m";

        // Load a/the laby.

        std::string name(argv[1]);

        if (std::filesystem::exists(name) && name.size() >= 6 && (name.substr(name.size() - 6, 6) == ".alaby" || name.substr(name.size() - 6, 6) == ".tlaby") && Tiles::Generater::Settings::ok(name)) {
            if (name[name.size() - 5] == 'a') {
                // A laby.

                ScreenPlaySetting screen_setting;
                screen_setting.load(name);
                screen_setting.go(window);
            } else {
                // The laby.

                // load

                std::ifstream file(name);

                Tiles::Generater::Settings setting;

                file >> setting;

                unsigned int seed = 0, p = 1;

                for (int i = 0; i < 4; ++i) {
                    seed += p * file.get();

                    p *= 256;
                }

                file.close();

                // play

                Tiles::Tile *tile = Tiles::Generater::generate(setting, seed);

                ScreenDraw game;

                game.go(tile, window, seed, setting);

                delete tile;
            }
        } else {
            std::cerr << "Error: no file or file is'n .alaby (.tlaby)" << std::endl;
        }
    } else {
        std::cerr << "Error: many arg. AStrangeLabyrinth OR AStrangeLabyrinth <path>" << std::endl;
    }

    window.close();

    Drawer::Setting::save();

    return 0;
}
