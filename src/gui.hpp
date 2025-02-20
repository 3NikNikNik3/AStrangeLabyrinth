#pragma once

#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace GUI {
    struct Scale {
        float x, y;
        int x_, y_;

        Scale(float x, int x_, float y, int y_);

        sf::Vector2f get(sf::Vector2u scale_window);
    };

	class Element {
	protected:
        Scale pos, scale;

    public:
        Element(Scale pos, Scale scale);

        virtual void draw(sf::RenderWindow& window, bool select) = 0;

        virtual void click(sf::Vector2u scale_window, float x, float y) = 0;

        virtual void keydown(sf::Keyboard::Scancode key) = 0;
	};

	class Button : public Element {
        bool active = false;

        sf::Texture texture;
        sf::Sprite spr;

    public:
        Button(Scale pos, Scale scale, std::string path);

        void draw(sf::RenderWindow& window, bool select);

        void click(sf::Vector2u scale_window, float x, float y);

        bool active_now();

        void keydown(sf::Keyboard::Scancode key);
	};
}
