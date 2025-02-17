#include "Screens.hpp"

#include "Drawer.hpp"

namespace AStrangeLabyrinth {
	namespace Screens {
		// ScreenDraw
		void ScreenDraw::go(Tiles::Tile* tile, sf::RenderWindow& window) {
            float a = 0;
            bool focus = true;

            const int h_x = 1, scale_x = 1, mouse_speed = 300;
            bool use_mouse = false;


            sf::View view(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
            window.setView(view);

            Vector pos = {1.5f, 1.5f};

            float add_speed = 1;

            sf::Clock clock;

            while (window.isOpen()) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>())
                        window.close();
                    else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x / scale_x, resized->size.y})));
                        if (use_mouse) sf::Mouse::setPosition({resized->size.x / 2, resized->size.y / 2}, window);
                    }
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        if (focus) {
                            if (key->scancode == sf::Keyboard::Scancode::LShift)
                                add_speed = 1.5;
                            else if (key->scancode == sf::Keyboard::Scancode::Escape) {
                                use_mouse = !use_mouse;
                                window.setMouseCursorVisible(!use_mouse);
                                sf::Mouse::setPosition({window.getSize().x / 2, window.getSize().y / 2}, window);
                            }
                        }
                    } else if (const auto* key = event->getIf<sf::Event::KeyReleased>()) {
                        if (key->scancode == sf::Keyboard::Scancode::LShift)
                            add_speed = 1;
                    } else if (event->is<sf::Event::FocusGained>())
                        focus = true;
                    else if (event->is<sf::Event::FocusLost>())
                        focus = false;
                }

                auto [w, h] = window.getSize();

                float delta =  clock.restart().asSeconds();

                if (focus) {
                    //a = Ray::mod_pi(a + 0.05);

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
                        a = Drawer::Ray::mod_pi(a + 2 * delta);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                        a = Drawer::Ray::mod_pi(a - 2 * delta);

                    if (use_mouse) {
                        a = Drawer::Ray::mod_pi(a + (sf::Mouse::getPosition(window).x - w / 2.0f) / mouse_speed);

                        sf::Mouse::setPosition({w / 2, h / 2}, window);
                    }

                    Vector shift(0, 0);

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                        shift.x += 1;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                        shift.y += 1;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                        shift.x -= 1;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                        shift.y -= 1;

                    if (use_mouse)
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                            shift.x += 1;
                        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                            shift.x -= 1;

                    Drawer::move_player(pos, tile, shift.rot(a).norm() * SPEED * add_speed * delta);

                    if (tile->end)
                        return;
                }

                window.clear({50, 50, 50});

                sf::RectangleShape rect({w / scale_x, h / 2});
                rect.setFillColor({100, 100, 100});

                window.draw(rect);

                Drawer::draw_see(tile, pos, a, Math::PI / 2, w / scale_x / h_x, w / scale_x, h, h_x, window);

                window.display();
            }
        }
	}
}

