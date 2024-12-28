#pragma once

#include <cmath>

#define uchar unsigned char


namespace AStrangeLabyrinth {
    namespace Math {
        const float PI = acos(-1);

        struct Vector {
            float x, y;

            Vector(float x, float y);

            Vector operator-(Vector ob);

            float ang();
        };

        struct Line {
            Vector a, b;

            Line(Vector a, Vector b);
        };
    }
}
