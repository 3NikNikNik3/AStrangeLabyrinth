#include "Screens.hpp"

#include <filesystem>
#include <fstream>
#include <ctime>

#ifdef __linux__
    #include <cstdio>
    #include <cstdlib>
#elif _WIN32
	#include <windows.h>
#endif

#include "Drawer.hpp"

namespace AStrangeLabyrinth {
	namespace Screens {
		// ScreenDraw
		sf::Font ScreenDraw::font("images/font.ttf");

		ScreenDraw::ScreenDraw() : text(font) {
            text.setFillColor({0, 255, 0});
            text.setPosition({2, 2});
            text.setCharacterSize(40);
		}

		bool ScreenDraw::go(Tiles::Tile* tile, sf::RenderWindow& window, unsigned int seed, Tiles::Generater::Settings& setting) {
            float a = 0;
            bool focus = true;

			window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {window.getSize().x / Drawer::Setting::scale_x, window.getSize().y})));

            Vector pos = {1.5f, 1.5f};

            float add_speed = 1;

            sf::Clock clock;

            if (Drawer::Setting::use_mouse) {
                sf::Mouse::setPosition({window.getSize().x / 2, window.getSize().y / 2}, window);
                window.setMouseCursorVisible(false);
            }

            while (window.isOpen()) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.setMouseCursorVisible(true);
                        return true;
                    }
                    else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x / Drawer::Setting::scale_x, resized->size.y})));
                        if (Drawer::Setting::use_mouse) sf::Mouse::setPosition({resized->size.x / 2, resized->size.y / 2}, window);
                    }
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        if (focus) {
                            if (key->scancode == sf::Keyboard::Scancode::LShift)
                                add_speed = 1.5;
                            else if (key->scancode == sf::Keyboard::Scancode::Escape) {
                                /*Drawer::Setting::use_mouse = !Drawer::Setting::use_mouse;
                                window.setMouseCursorVisible(!Drawer::Setting::use_mouse);
                                sf::Mouse::setPosition({window.getSize().x / 2, window.getSize().y / 2}, window);*/

                                if (Drawer::Setting::use_mouse) {
                                    sf::Mouse::setPosition({window.getSize().x / 2, window.getSize().y / 2}, window);
                                    window.setMouseCursorVisible(true);
                                }

								window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {window.getSize().x, window.getSize().y})));

                                if (uchar what = pause.go(window, seed, setting))
                                    return what - 1 == 1;

								window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {window.getSize().x / Drawer::Setting::scale_x, window.getSize().y})));

                                if (Drawer::Setting::use_mouse) {
                                    sf::Mouse::setPosition({window.getSize().x / 2, window.getSize().y / 2}, window);
                                    window.setMouseCursorVisible(false);
                                }

                                clock.restart();
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

                    if (Drawer::Setting::use_mouse) {
                        a = Drawer::Ray::mod_pi(a + (sf::Mouse::getPosition(window).x - w / 2.0f) / Drawer::Setting::mouse_speed);

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

                    if (Drawer::Setting::use_mouse)
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                            shift.x += 1;
                        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                            shift.x -= 1;

                    Drawer::move_player(pos, tile, shift.rot(a).norm() * SPEED * add_speed * delta);

                    if (tile->end) {
                        window.setMouseCursorVisible(true);
						window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {window.getSize().x, window.getSize().y})));
                        return false;
                    }
                }

                // Clear window
                window.clear({50, 50, 50});

                sf::RectangleShape rect({w / Drawer::Setting::scale_x, h / 2});
                rect.setFillColor({100, 100, 100});

                window.draw(rect);

                // Draw rooms
                Drawer::draw_see(tile, pos, a, Math::PI / 2, w / Drawer::Setting::scale_x / Drawer::Setting::h_x, w / Drawer::Setting::scale_x, h, Drawer::Setting::h_x, window);

                // Draw fps
                if (Drawer::Setting::view_fps) {
                    text.setString(std::to_string((int)(1 / delta)));
                    window.draw(text);
                }

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
                                     exit({0.5, 0, 0.85, 0}, {0.5, 0, 0.1, 0, true}, "images/exit.png"),
                                     link({1, -25, 1, -25}, {0, 40, 0, 40}, "images/github.png"),
                                     how_control_but({0, 25, 1, -25}, {0, 40, 0, 40}, "images/keyboard.png") {
            arr.push_back(&play);
            arr.push_back(&play_setting);
            arr.push_back(&settings);
            arr.push_back(&exit);
            arr.push_back(&how_control_but);
            arr.push_back(&link);
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
                    Tiles::Generater::Settings setting(1, 1, 1, 3, 2, 1, 1, 0);
                    std::pair<Tiles::Tile*, unsigned int> room = Tiles::Generater::generate(setting);
                    bool res = main_loop.go(room.first, window, room.second, setting);
                    delete room.first;

                    if (res)
                        return;
                }

                if (play_setting.active_now()) {
                    if (play_setting_screen.go(window))
                        return;
                }

                if (settings.active_now()) {
                    if (setting_screen.go(window))
                        return;
                }

                if (exit.active_now())
                    return;

                if (link.active_now()) {
                    #ifdef __linux__
                        std::system("xdg-open https://github.com/3NikNikNik3/AStrangeLabyrinth");
                    #elif _WIN32
                        ShellExecute(NULL, NULL, "https://github.com/3NikNikNik3/AStrangeLabyrinth", NULL, NULL, SW_SHOW);
                    #endif
                }

                if (how_control_but.active_now())
                    if (how_control.go(window))
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
                                                               GUI::Number({0.25, 0, 0.76, 50}, {0, 200, 0, 100}, 0, 254, 0, "images/count_forks2.png") }),
                                                 ends({ GUI::Number({0.75, 0, 3.0/11, 50}, {0, 200, 0, 100}, 0, 254, 0, "images/end_nothing.png"),
                                                        GUI::Number({0.75, 0, 5.0/11, 50}, {0, 200, 0, 100}, 0, 254, 0, "images/end_cor.png"),
                                                        GUI::Number({0.75, 0, 7.0/11, 50}, {0, 200, 0, 100}, 0, 254, 0, "images/end_rot.png"),
                                                        GUI::Number({0.75, 0, 9.0/11, 50}, {0, 200, 0, 100}, 0, 254, 0, "images/end_fake.png") }),
                                                 end_what_tex("images/what_end.png"), end_what(end_what_tex) {
                end_what.setScale({100.0 / end_what_tex.getSize().x, 100.0 / end_what_tex.getSize().y});

                arr.push_back(&back_but);
                arr.push_back(&load_but);

                arr.push_back(&start_tile);

                for (int i = 0; i < 3; ++i)
                    arr.push_back(&count_forks[i]);

                for (int i = 0; i < 4; ++i)
                    arr.push_back(&ends[i]);

                arr.push_back(&save_but);
                arr.push_back(&play);

                if (std::filesystem::exists("data/last.alaby") && Tiles::Generater::Settings::ok("data/last.alaby")) {
                    load("data/last.alaby");
                } else {
                    start_tile.val = setting.count_start_forks;
                    for (int i = 0; i < 3; ++i)
                        count_forks[i].val = setting.depth_forks[i];
                    for (int i = 0; i < 4; ++i)
                        ends[i].val = setting.end_event[i];
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
            for (int i = 0; i < 4; ++i)
                ends[i].val = setting.end_event[i];

            int i = from.size() - 1;
            while (i > -1 && from[i] != '/' && from[i] != '\\') --i;

            name = from.substr(i + 1, from.size() - i - 1);
        }

        unsigned int ScreenPlaySetting::load_with_seed(std::string from) {
            std::ifstream file(from);

            file >> setting;

            unsigned int res = 0, p = 1;

            for (int i = 0; i < 4; ++i) {
                res += p * file.get();

                p *= 256;
            }

            file.close();

            start_tile.val = setting.count_start_forks;
            for (int i = 0; i < 3; ++i)
                count_forks[i].val = setting.depth_forks[i];
            for (int i = 0; i < 4; ++i)
                ends[i].val = setting.end_event[i];

            int i = from.size() - 1;
            while (i > -1 && from[i] != '/' && from[i] != '\\') --i;

            name = from.substr(i + 1, from.size() - i - 1);
            name[name.size() - 5] = 'a';

            return res;
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
			ofn.lpstrFilter = "A/The labyrinth\0*.alaby;*.tlaby\0";
			ofn.nFilterIndex = 1;

			if (GetOpenFileName(&ofn))
				res = std::string(ofn.lpstrFile);

			#endif

            if (res != "" && std::filesystem::exists(res))
                return res;
            return "";
        }

        bool ScreenPlaySetting::go(sf::RenderWindow& window) {
            now_select = 0;

            while (true) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>())
                        return true;
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        keydown(key->scancode);

                        if (key->scancode == sf::Keyboard::Scancode::Enter && now_select == 1)
                            now_select = arr.size() - 1;
                        else if (key->scancode == sf::Keyboard::Scancode::Escape)
                            return false;
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
                int sum_ends = 0;
                for (int i = 0; i < 4; ++i) {
                    setting.end_event[i] = ends[i].val;
                    sum_ends += ends[i].val;
                }

                if (sum_ends == 0) {
                    setting.sum_end_event = 1;
                    setting.end_event[0] = 1;
                    ends[0].val = 1;
                } else
                    setting.sum_end_event = sum_ends;

                if (back_but.active_now()) {
                    return false;
                }

                if (load_but.active_now()) {
                    std::string new_name = chose_file();
                    if (new_name != "" && new_name.size() >= 6 && new_name.substr(new_name.size() - 6, 6) == ".alaby" && Tiles::Generater::Settings::ok(new_name)) {
                        load(new_name);
                    } else if (new_name != "" && new_name.size() >= 6 && new_name.substr(new_name.size() - 6, 6) == ".tlaby" && Tiles::Generater::Settings::ok(new_name)) {
                        unsigned int seed = load_with_seed(new_name);

                        Tiles::Tile *room = Tiles::Generater::generate(setting, seed);

                        bool res = main_loop.go(room, window, seed, setting);

                        delete room;

                        if (res)
                            return true;
                        return false;
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

                    std::pair<Tiles::Tile*, unsigned int> room = Tiles::Generater::generate(setting);

                    bool res = main_loop.go(room.first, window, room.second, setting);

                    delete room.first;

                    if (res)
                        return true;
                    return false;
                }

                window.clear({200, 200, 200});

                draw(window);

                end_what.setPosition({ 0.75 * window.getSize().x - 50, 1.0/11 * window.getSize().y });
                window.draw(end_what);

                window.display();
            }
        }

        using Setting = Drawer::Setting;

        // ScreenSetting
        ScreenSetting::ScreenSetting() : back_but({0, 20, 0, 20}, {0, 30, 0, 30}, "images/back.png"),
                                         h_x({0.25, 0, 1/5.0, 0}, {0, 200, 0, 100}, 1, 255, 1, "images/h_x.png"),
                                         scale_x({0.25, 0, 3/5.0, 0}, {0, 200, 0, 100}, 1, 255, 1, "images/scale_x.png"),
                                         mouse_speed({0.5, 0, 2/5.0, 0}, {0, 200, 0, 100}, 1, 65535, 100, "images/mouse_speed.png"),
                                         use_mouse({0.5, 0, 4/5.0, 0}, {0, 200, 0, 100}, {"images/no.png", "images/yes.png"}, "images/use_mouse.png"),
                                         fps({0.75, 0, 1/5.0, 0}, {0, 200, 0, 100}, 0, 255, 60, "images/fps.png"),
                                         view_fps({0.75, 0, 3/5.0, 0}, {0, 200, 0, 100}, {"images/no.png", "images/yes.png"}, "images/view_fps.png") {
            arr.push_back(&back_but);
            arr.push_back(&h_x);
            arr.push_back(&scale_x);
            arr.push_back(&mouse_speed);
            arr.push_back(&use_mouse);
            arr.push_back(&fps);
            arr.push_back(&view_fps);
        }

        bool ScreenSetting::go(sf::RenderWindow& window) {
            now_select = 0;

            h_x.val = Setting::h_x;
            scale_x.val = Setting::scale_x;
            mouse_speed.val = Setting::mouse_speed;
            use_mouse.choice = Setting::use_mouse;
            fps.val = Setting::fps;
            view_fps.choice = Setting::view_fps;

            while (true) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>())
                        return true;
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        if (key->scancode == sf::Keyboard::Scancode::Escape)
                            return false;
                        else
                            keydown(key->scancode);
                    }
                    else if (const auto* resized = event->getIf<sf::Event::Resized>())
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x, resized->size.y})));
                    else if (const auto* but = event->getIf<sf::Event::MouseButtonPressed>())
                        if (but->button == sf::Mouse::Button::Left)
                            click(window, but->position.x, but->position.y);
                }

                if (back_but.active_now()) {
                    Setting::h_x = h_x.val;
                    Setting::scale_x = scale_x.val;
                    Setting::mouse_speed = mouse_speed.val;
                    Setting::use_mouse = use_mouse.choice == 1;
                    Setting::view_fps = view_fps.choice == 1;

                    window.setFramerateLimit(fps.val);
                    Setting::fps = fps.val;

                    Setting::save();

                    return false;
                }

                window.clear({200, 200, 200});

                draw(window);

                window.display();
            }
        }

        // ScreenPause
        ScreenPause::ScreenPause() : play({0.5, 0, 2/7.0, 0}, {0.5, 0, 0.1, 0, true}, "images/play.png"),
                                     setting_but({0.5, 0, 4/7.0, 0}, {0.5, 0, 0.1, 0, true}, "images/settings.png"),
                                     exit({0.5, 0, 6/7.0, 0}, {0.5, 0, 0.1, 0, true}, "images/exit.png"),
                                     save({1, -30, 1, -30}, {0, 50, 0, 50}, "images/save.png"),
                                     use_mouse({0, 110, 1, -55}, {0, 200, 0, 100}, {"images/no.png", "images/yes.png"}, "images/use_mouse.png") {
            arr.push_back(&play);
            arr.push_back(&use_mouse);
            arr.push_back(&setting_but);
            arr.push_back(&exit);
            arr.push_back(&save);
        }

        uchar ScreenPause::go(sf::RenderWindow& window, unsigned int seed, Tiles::Generater::Settings& setting) {
            use_mouse.choice = Setting::use_mouse;

            now_select = 0;

            bool move_ = true;

            while (true) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>())
                        return 2;
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        keydown(key->scancode);

                        if (key->scancode == sf::Keyboard::Scancode::Enter && now_select == 1 && move_) {
                            now_select = 0;
                            move_ = false;
                        } else if (key->scancode == sf::Keyboard::Scancode::Escape) {
                            Setting::use_mouse = use_mouse.choice;
                            Setting::save();

                            return 0;
                        }
                    }
                    else if (const auto* resized = event->getIf<sf::Event::Resized>())
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x, resized->size.y})));
                    else if (const auto* but = event->getIf<sf::Event::MouseButtonPressed>())
                        if (but->button == sf::Mouse::Button::Left)
                            click(window, but->position.x, but->position.y);
                }

                if (play.active_now()) {
                    Setting::use_mouse = use_mouse.choice;
                    Setting::save();

                    return 0;
                }

                if (exit.active_now()) {
                    return 1;
                }

                if (setting_but.active_now()) {
                    Setting::use_mouse = use_mouse.choice;
                    if (setting_screen.go(window))
                        return 2;
                    use_mouse.choice = Setting::use_mouse;

                    now_select = 0;
                }

                if (save.active_now()) {
                    tm date_now;
                    std::time_t time_now = time(0);

                    date_now = *std::localtime(&time_now);

                    std::string name = std::to_string(date_now.tm_year + 1900) + "." + std::to_string(date_now.tm_mon + 1) + "." +
                                       std::to_string(date_now.tm_mday) + "." + std::to_string(date_now.tm_hour) + "." +
                                       std::to_string(date_now.tm_min) + "." + std::to_string(date_now.tm_sec);

                    while (std::filesystem::exists(name + ".tlaby"))
                        name += "_";

                    name += ".tlaby";

                    if (!std::filesystem::exists("data"))
                        std::filesystem::create_directories("data");

                    std::ofstream file("data/" + name);

                    file << setting;

                    for (int i = 0, seed_ = seed; i < 4; ++i) {
                        file << (uchar)(seed_ % 256);

                        seed_ /= 256;
                    }

                    file.close();
                }

                window.clear({200, 200, 200});

                draw(window);

                window.display();
            }
        }

        // ScreenHowControl
        ScreenHowControl::ScreenHowControl() : back_but({0, 15, 0, 15}, {0, 20, 0, 20}, "images/back.png"),
                                               image("images/how_control.png"), sprite(image) {
            arr.push_back(&back_but);

            //std::cout << sprite.getTexture().getSize().x << ' ' << sprite.getTexture().getSize().y << std::endl;
        }

        bool ScreenHowControl::go(sf::RenderWindow& window) {
            sprite.setScale({window.getSize().x / (float)image.getSize().x, window.getSize().y / (float)image.getSize().y});

            while (true) {
                while (const std::optional event = window.pollEvent())
                    if (event->is<sf::Event::Closed>())
                        return true;
                    else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                        keydown(key->scancode);

                        if (key->scancode == sf::Keyboard::Scancode::Escape)
                            return false;
                    } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                        window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {resized->size.x, resized->size.y})));

                        sprite.setScale({window.getSize().x / (float)image.getSize().x, window.getSize().y / (float)image.getSize().y});
                    }
                    else if (const auto* but = event->getIf<sf::Event::MouseButtonPressed>())
                        if (but->button == sf::Mouse::Button::Left)
                            click(window, but->position.x, but->position.y);

                if (back_but.active_now())
                    return false;

                window.clear({200, 200, 200});
                window.draw(sprite);

                draw(window);


                window.display();
            }
        }
	}
}

