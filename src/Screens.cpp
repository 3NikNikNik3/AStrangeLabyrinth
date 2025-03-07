#include "Screens.hpp"

#include <filesystem>
#include <fstream>
#include <ctime>
#include <iostream>

#ifdef __linux__
    #include <cstdio>
#elif _WIN32
	#include <windows.h>
	//#include <commdlg.h>
#endif

#include "Drawer.hpp"

namespace AStrangeLabyrinth {
	namespace Screens {
		// ScreenDraw
		bool ScreenDraw::go(Tiles::Tile* tile, sf::RenderWindow& window) {
            float a = 0;
            bool focus = true;

            const int h_x = 1, scale_x = 1, mouse_speed = 300;
            bool use_mouse = false;


            /*sf::View view(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
            window.setView(view);*/

            Vector pos = {1.5f, 1.5f};

            float add_speed = 1;

            sf::Clock clock;

            while (window.isOpen()) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.setMouseCursorVisible(true);
                        return true;
                    }
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

                    if (tile->end) {
                        window.setMouseCursorVisible(true);
                        return false;
                    }
                }

                window.clear({50, 50, 50});

                sf::RectangleShape rect({w / scale_x, h / 2});
                rect.setFillColor({100, 100, 100});

                window.draw(rect);

                Drawer::draw_see(tile, pos, a, Math::PI / 2, w / scale_x / h_x, w / scale_x, h, h_x, window);

                window.display();
            }
        }

        // ScreenWithGUI

        void ScreenWithGUI::draw(sf::RenderWindow& window) {
            now_select %= arr.size();

            for (int i = 0; i < arr.size(); ++i) {
                arr[i]->draw(window, now_select == i);
            }
        }

        void ScreenWithGUI::click(sf::RenderWindow& window, int x, int y) {
            now_select %= arr.size();

            sf::Vector2u scale_window = window.getSize();
            for (int i = 0; i < arr.size(); ++i) {
                arr[i]->click(scale_window, x, y);
            }
        }

        void ScreenWithGUI::keydown(sf::Keyboard::Scancode key) {
            now_select %= arr.size();

            if (key == sf::Keyboard::Scancode::Tab)
                now_select = (now_select + 1) % arr.size();
            else
                arr[now_select]->keydown(key);
        }

        // ScreenStart
        ScreenStart::ScreenStart() : play({0.5, 0, 0.25, 0}, {0.5, 0, 0.1, 0, true}, "images/play.png"),
                                     play_setting({0.5, 0, 0.45, 0}, {0.5, 0, 0.1, 0, true}, "images/play_setting.png"),
                                     settings({0.5, 0, 0.65, 0}, {0.5, 0, 0.1, 0, true}, "images/settings.png"),
                                     exit({0.5, 0, 0.85, 0}, {0.5, 0, 0.1, 0, true}, "images/exit.png") {
            arr.push_back(&play);
            arr.push_back(&play_setting);
            arr.push_back(&settings);
            arr.push_back(&exit);
        }

        void ScreenStart::go(sf::RenderWindow& window) {
            while (true) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>())
                        return;
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                        keydown(key->scancode);
                    else if (const auto* resized = event->getIf<sf::Event::Resized>())
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x, resized->size.y})));
                    else if (const auto* but = event->getIf<sf::Event::MouseButtonPressed>())
                        if (but->button == sf::Mouse::Button::Left)
                            click(window, but->position.x, but->position.y);
                }

                if (play.active_now()) {
                    Tiles::Tile *room = Tiles::Generater::generate(Tiles::Generater::Settings(1, 1, 1, 3));
                    bool res = main_loop.go(room, window);
                    delete room;

                    if (res)
                        return;
                }

                if (play_setting.active_now()) {
                    if (play_setting_screen.go(window))
                        return;
                }

                if (settings.active_now()) {

                }

                if (exit.active_now())
                    return;

                window.clear({200, 200, 200});

                draw(window);

                window.display();
            }
        }

        // ScreenPlaySetting
        ScreenPlaySetting::ScreenPlaySetting() : name("last.alaby"),
                                                 back_but({0, 35, 0, 35}, {0, 50, 0, 50, true}, "images/back.png"),
                                                 load_but({1, -35, 0, 35}, {0, 50, 0, 50, true}, "images/load.png"),
                                                 save_but({1, -95, 0, 35}, {0, 50, 0, 50, true}, "images/save.png"),
                                                 play({1, -155, 0, 35}, {0, 50, 0, 50, true}, "images/play.png"),
                                                 start_tile({0.25, 0, 0.19, 50}, {0, 200, 0, 100}, 1, 4, 0, "images/start_tile.png"),
                                                 count_forks({ GUI::Number({0.25, 0, 0.38, 50}, {0, 200, 0, 100}, 0, 254, 0, "images/count_forks0.png"),
                                                               GUI::Number({0.25, 0, 0.57, 50}, {0, 200, 0, 100}, 0, 254, 0, "images/count_forks1.png"),
                                                               GUI::Number({0.25, 0, 0.76, 50}, {0, 200, 0, 100}, 0, 254, 0, "images/count_forks2.png")}) {
                arr.push_back(&back_but);
                arr.push_back(&load_but);

                arr.push_back(&start_tile);

                for (int i = 0; i < 3; ++i)
                    arr.push_back(&count_forks[i]);

                arr.push_back(&save_but);
                arr.push_back(&play);

                if (std::filesystem::exists("data/last.alaby") && Tiles::Generater::Settings::ok("data/last.alaby")) {
                    load("data/last.alaby");
                } else {
                    start_tile.val = setting.count_start_forks;
                    for (int i = 0; i < 3; ++i)
                        count_forks[i].val = setting.depth_forks[i];
                }

                #ifdef __linux__

                path_me = std::filesystem::current_path();

                #endif
        }

        void ScreenPlaySetting::save() {
            if (!std::filesystem::exists("data"))
                std::filesystem::create_directories("data");

            std::ofstream file("data/" + name);
            file << setting;
            file.close();
        }

        void ScreenPlaySetting::load(std::string from) {
            std::ifstream file(from);

            file >> setting;

            file.close();

            start_tile.val = setting.count_start_forks;
            for (int i = 0; i < 3; ++i)
                count_forks[i].val = setting.depth_forks[i];

            int i = from.size() - 1;
            while (i > -1 && from[i] != '/' && from[i] != '\\') --i;

            name = from.substr(i + 1, from.size() - i - 1);
        }

        std::string ScreenPlaySetting::chose_file() {
            std::string res = "";

            #ifdef __linux__

            std::string command = "zenity --file-selection --file-filter=*.?laby --filename=" + path_me;
            FILE *chose = popen(command.data(), "r");
            char buf[1024];

            while (fgets(buf, sizeof(buf), chose) != nullptr)
                res += std::string(buf);

            pclose(chose);

            res = res.substr(0, res.size() - 1);

            #elif _WIN32
			
			OPENFILENAME ofn;
			char szFile[1024];
			HWND hwnd = nullptr;
			
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = szFile;
			
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			
			if (GetOpenFileName(&ofn))
				res = std::string(ofn.lpstrFile);
						
			#endif

            if (res != "" && std::filesystem::exists(res))
                return res;
            return "";
        }

        bool ScreenPlaySetting::go(sf::RenderWindow& window) {
            while (true) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>())
                        return true;
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        keydown(key->scancode);

                        if (key->scancode == sf::Keyboard::Scancode::Enter && now_select == 1)
                            now_select = arr.size() - 1;
                    }
                    else if (const auto* resized = event->getIf<sf::Event::Resized>())
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x, resized->size.y})));
                    else if (const auto* but = event->getIf<sf::Event::MouseButtonPressed>())
                        if (but->button == sf::Mouse::Button::Left)
                            click(window, but->position.x, but->position.y);
                }

                setting.count_start_forks = start_tile.val;
                for (int i = 0; i < 3; ++i)
                    setting.depth_forks[i] = count_forks[i].val;

                if (back_but.active_now()) {
                    return false;
                }

                if (load_but.active_now()) {
                    std::string new_name = chose_file();
                    if (new_name != "" && new_name.size() >= 6 && new_name.substr(new_name.size() - 6, 6) == ".alaby" && Tiles::Generater::Settings::ok(new_name)) {
                        load(new_name);
                    }
                }

                if (save_but.active_now()) {
                    if (name == "last.alaby") {
                        tm date_now;
                        std::time_t time_now = time(0);

                        date_now = *std::localtime(&time_now);

                        name = std::to_string(date_now.tm_year + 1900) + "." + std::to_string(date_now.tm_mon + 1) + "." +
                               std::to_string(date_now.tm_mday) + "." + std::to_string(date_now.tm_hour) + "." +
                               std::to_string(date_now.tm_min) + "." + std::to_string(date_now.tm_sec);

                        while (std::filesystem::exists(name + ".alaby"))
                            name += "_";

                        name += ".alaby";
                    }

                    save();
                }

                if (play.active_now()) {
                    save();

                    Tiles::Tile *room = Tiles::Generater::generate(setting);

                    bool res = main_loop.go(room, window);

                    delete room;

                    if (res)
                        return true;
                    return false;
                }

                window.clear({200, 200, 200});

                draw(window);

                window.display();
            }
        }
	}
}

