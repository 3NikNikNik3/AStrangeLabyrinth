#pragma once

#include <cmath>

#define uchar unsigned char


namespace AStrangeLabyrinth {
    namespace Math {
        const float PI = acos(-1);

        struct Vector {
            float x, y;

            Vector(float x, float y);

            Vector operator+(Vector ob);

            void operator+=(Vector ob);

            Vector operator-(Vector ob);

            float operator*(Vector ob);

            Vector operator*(float val);

            float len();

            float ang();

            Vector rot90();

            Vector norm();

            Vector rot(float a);
        };

        struct Line {
            Vector a, b;

            Line(Vector a, Vector b);
        };
    }
}
