#include "Screens.hpp"

#include "Drawer.hpp"

namespace AStrangeLabyrinth {
	namespace Screens {
		// ScreenDraw
		void ScreenDraw::go(Tiles::Tile* tile, sf::RenderWindow& window) {
            float a = 0;

            const int h_x = 2, scale_x = 2;

            sf::View view(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
            window.setView(view);

            Vector pos = {1.5f, 1.5f};

            float add_speed = 1;

            sf::Clock clock;

            while (window.isOpen()) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>())
                        window.close();
                    else if (const auto* resized = event->getIf<sf::Event::Resized>())
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x / scale_x, resized->size.y})));
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        if (key->scancode == sf::Keyboard::Scancode::LShift)
                            add_speed = 1.5;
                    } else if (const auto* key = event->getIf<sf::Event::KeyReleased>()) {
                        if (key->scancode == sf::Keyboard::Scancode::LShift)
                            add_speed = 1;
                    }
                }

                float delta =  clock.restart().asSeconds();

                //a = Ray::mod_pi(a + 0.05);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
                    a = Drawer::Ray::mod_pi(a + 2 * delta);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                    a = Drawer::Ray::mod_pi(a - 2 * delta);

                Vector shift(0, 0);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    shift.x += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                    shift.y += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    shift.x -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                    shift.y -= 1;

                Drawer::move_player(pos, tile, shift.rot(a).norm() * SPEED * add_speed * delta);

                auto [w, h] = window.getSize();
                window.clear(sf::Color::White);

                Drawer::draw_see(tile, pos, a, Math::PI / 2, w / scale_x / h_x, w / scale_x, h, window);

                window.display();
            }
        }
	}
}

