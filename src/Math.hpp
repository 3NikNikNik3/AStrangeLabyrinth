#pragma once

#define uchar unsigned char


namespace AStrangeLabyrinth {
    namespace Math {
        struct Vector {
            float x, y;

            Vector(float x, float y);
        };

        struct Line {
            Vector a, b;

            Line(Vector a, Vector b);
        };
    }
}
