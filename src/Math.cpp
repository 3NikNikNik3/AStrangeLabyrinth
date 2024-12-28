#include "Math.hpp"

namespace AStrangeLabyrinth {
    namespace Math {

        // Vector
        Vector::Vector(float x, float y) : x(x), y(y) {}

        Vector Vector::operator-(Vector ob) {
            return {x - ob.x, y - ob.y};
        }

        float Vector::ang() {
            return atan2(y, x);
        }

        // Line
        Line::Line(Vector a, Vector b) : a(a), b(b) {}

    }
}

