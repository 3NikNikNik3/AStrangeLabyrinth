#include "gui.hpp"

namespace GUI {
	// Scale
    Scale::Scale(float x, int x_, float y, int y_, bool connect) : x(x), x_(x_), y(y), y_(y_), connect(connect) {}

    sf::Vector2f Scale::get(sf::Vector2u scale_window) {
        if (connect) {
            float ans = std::min(x_ + x * scale_window.x, y_ + y * scale_window.y);
            return {ans, ans};
        }

        return {x_ + x * scale_window.x, y_ + y * scale_window.y};
    }

    // Element
    Element::Element(Scale pos, Scale scale) : pos(pos), scale(scale) {}

    // Button
    Button::Button(Scale pos, Scale scale, std::string path) : Element(pos, scale), texture(path), spr(texture) {}

    sf::Vector2f get_scale(sf::Vector2f will, sf::Vector2u now) {
        return { will.x / now.x, will.y / now.y} ;
    }

    void Button::draw(sf::RenderWindow& window, bool select) {
        if (select) {
            sf::RectangleShape rect(scale.get(window.getSize()) + sf::Vector2f(6, 6));
            rect.setFillColor({0, 255, 0});
            rect.setPosition(pos.get(window.getSize()) - scale.get(window.getSize()) / 2.0f - sf::Vector2f(3, 3));
            window.draw(rect);
        }

        spr.setPosition(pos.get(window.getSize()) - scale.get(window.getSize()) / 2.0f);
        spr.setScale(get_scale(scale.get(window.getSize()), texture.getSize()));
        window.draw(spr);
    }

    void Button::click(sf::Vector2u scale_window, float x, float y) {
        sf::Vector2f now_pos = pos.get(scale_window), scale_now = scale.get(scale_window);
        x -= now_pos.x;
        y -= now_pos.y;

        if (-scale_now.x / 2 <= x && x <= scale_now.x / 2 && -scale_now.y / 2 <= y && y <= scale_now.y / 2)
            active = true;
    }

    bool Button::active_now() {
        if (active) {
            active = false;
            return true;
        }
        return false;
    }

    void Button::keydown(sf::Keyboard::Scancode key) {
        if (key == sf::Keyboard::Scancode::Enter)
            active = true;
    }

    // Check
    Check::Check(Scale pos, Scale scale, std::vector<std::string> paths, std::string path_name) : Element(pos, scale), name_texture(path_name), name_sprite(name_texture), choice(0) {
        texturs.resize(paths.size());
        for (int i = 0; i < paths.size(); ++i) {
            texturs[i].loadFromFile(paths[i]);
        }

        sprits.resize(paths.size(), sf::Sprite(texturs[0]));
        for (int i = 1; i < paths.size(); ++i)
            sprits[i].setTexture(texturs[i]);
    }

    void Check::draw(sf::RenderWindow& window, bool select) {
        sf::Vector2f pos_ = pos.get(window.getSize()), scale_ = scale.get(window.getSize());

        pos_ -= scale_ / 2.0f;

        // Draw Name

        name_sprite.setPosition(pos_);
        name_sprite.setScale(get_scale({scale_.x / 2 - 10, scale_.y}, name_texture.getSize()));
        window.draw(name_sprite);

        // Draw choice

        if (select) {
            sf::RectangleShape rect({scale_.x / 2 - 4, scale_.y + 6});
            rect.setFillColor({0, 255, 0});
            rect.setPosition(pos_ - sf::Vector2f(3 - 5 - scale_.x / 2.0f, 3));
            window.draw(rect);
        }
        sprits[choice].setPosition(pos_ + sf::Vector2f(5 + scale_.x / 2.0f, 0));
        sprits[choice].setScale(get_scale({scale_.x / 2 - 10, scale_.y}, texturs[choice].getSize()));
        window.draw(sprits[choice]);
    }

    void Check::click(sf::Vector2u scale_window, float x, float y) {
        sf::Vector2f pos_ = pos.get(scale_window), scale_ = scale.get(scale_window);

        if (pos_.x + 5 <= x && x <= pos_.x + scale_.x / 2.0f && pos_.y - scale_.y / 2.0f <= y && y <= pos_.y + scale_.y / 2.0f)
            choice = (choice + 1) % texturs.size();
    }

    void Check::keydown(sf::Keyboard::Scancode key) {
        if (key == sf::Keyboard::Scancode::Left)
            choice = (choice - 1) % texturs.size();
        else if (key == sf::Keyboard::Scancode::Right || key == sf::Keyboard::Scancode::Enter)
            choice = (choice + 1) % texturs.size();
    }

    // Number
    sf::Font Number::font("images/font.ttf");

    Number::Number(Scale pos, Scale scale, unsigned int min_val, unsigned int max_val, unsigned int val, std::string name_texture_path) :
        Element(pos, scale), min_val(min_val), max_val(max_val), val(val), name_texture(name_texture_path), name_sprite(name_texture), text(font),
        add({pos.x + scale.x / 4, pos.x_ + scale.x_ / 4, pos.y - scale.y / 3, pos.y_ - scale.y_ / 3}, {scale.x / 2, scale.x_ / 2, scale.y / 3, scale.y_ / 3}, "images/add.png"),
        minus_button({pos.x + scale.x / 4, pos.x_ + scale.x_ / 4, pos.y + scale.y / 3, pos.y_ + scale.y_ / 3}, {scale.x / 2, scale.x_ / 2, scale.y / 3, scale.y_ / 3}, "images/minus.png"){}

    void Number::draw(sf::RenderWindow& window, bool select) {
        // Draw Button
        add.draw(window, false);
        minus_button.draw(window, false);

        sf::Vector2f pos_ = pos.get(window.getSize()), scale_ = scale.get(window.getSize());

        // Draw Name

        name_sprite.setPosition(pos_ - sf::Vector2f(scale_.x / 2, scale_.y / 2));
        name_sprite.setScale(get_scale({scale_.x / 2 - 5, scale_.y}, name_texture.getSize()));
        window.draw(name_sprite);

        // Draw Text

        // Select
        if (select) {
            sf::RectangleShape rect({scale_.x / 2 + 4, scale_.y / 3 - 5});
            rect.setFillColor({0, 255, 0});
            rect.setPosition(pos_ + sf::Vector2f(-2, -scale_.y / 6 + 2));
            window.draw(rect);
        }

        // Background
        sf::RectangleShape rect_text({scale_.x / 2, scale_.y / 3 - 10});
        rect_text.setFillColor({180, 180, 180});
        rect_text.setPosition(pos_ + sf::Vector2f(0, -scale_.y / 6 + 5));
        window.draw(rect_text);

        // Text
        text.setString(std::to_string(val));
        text.setPosition(pos_ + sf::Vector2f(scale_.x / 4 - std::min(scale_.x, (scale_.y / 3 - 10) * text.getString().getSize() / 4), -scale_.y / 6 + 2));
        text.setCharacterSize(scale_.y / 3 - 10);
        window.draw(text);
    }

    void Number::click(sf::Vector2u scale_window, float x, float y) {
        add.click(scale_window, x, y);

        if (add.active_now() && val < max_val)
            ++val;

        minus_button.click(scale_window, x, y);

        if (minus_button.active_now() && val > min_val)
            --val;
    }

    void Number::keydown(sf::Keyboard::Scancode key) {
        if (key == sf::Keyboard::Scancode::Up && val < max_val)
            ++val;
        else if (key == sf::Keyboard::Scancode::Down && val > min_val)
            --val;
    }
}
