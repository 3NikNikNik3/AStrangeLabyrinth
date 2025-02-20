#include "gui.hpp"

namespace GUI {
	// Scale
    Scale::Scale(float x, int x_, float y, int y_) : x(x), x_(x_), y(y), y_(y_) {}

    sf::Vector2f Scale::get(sf::Vector2u scale_window) {
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
}
