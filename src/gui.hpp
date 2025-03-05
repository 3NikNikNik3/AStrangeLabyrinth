#pragma once

#include <string>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define uchar unsigned char

namespace GUI {
    struct Scale {
        float x, y;
        int x_, y_;
        bool connect;

        Scale(float x, int x_, float y, int y_, bool connect=false);

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
        // please, 1x1
        Button(Scale pos, Scale scale, std::string path);

        void draw(sf::RenderWindow& window, bool select);

        void click(sf::Vector2u scale_window, float x, float y);

        bool active_now();

        void keydown(sf::Keyboard::Scancode key);
	};

    class Check : public Element {
        std::vector<sf::Texture> texturs;
        std::vector<sf::Sprite> sprits;

        sf::Texture name_texture;
        sf::Sprite name_sprite;

    public:
        uchar choice;

        // please, 2x1
        Check(Scale pos, Scale scale, std::vector<std::string> paths, std::string path_name);

        void draw(sf::RenderWindow& window, bool select);

        void click(sf::Vector2u scale_window, float x, float y);

        void keydown(sf::Keyboard::Scancode key);
    };

    class Number : public Element {
        static sf::Font font;

        Button add, minus_button;

        sf::Texture name_texture;
        sf::Sprite name_sprite;

        sf::Text text;

    public:
        unsigned int min_val, max_val, val;

        // please, 2x1
        Number(Scale pos, Scale scale, unsigned int min_val, unsigned int max_val, unsigned int val, std::string name_texture_path);

        void draw(sf::RenderWindow& window, bool select);

        void click(sf::Vector2u scale_window, float x, float y);

        void keydown(sf::Keyboard::Scancode key);
    };
}
