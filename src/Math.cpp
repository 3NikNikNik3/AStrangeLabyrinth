#include "Math.hpp"

namespace AStrangeLabyrinth {
    namespace Math {

        // Vector
        Vector::Vector(float x, float y) : x(x), y(y) {}

        Vector Vector::operator+(Vector ob) {
            return {x + ob.x, y + ob.y};
        }

        void Vector::operator+=(Vector ob) {
            x += ob.x;
            y += ob.y;
        }

        Vector Vector::operator-(Vector ob) {
            return {x - ob.x, y - ob.y};
        }

        float Vector::operator*(Vector ob) {
            return x * ob.x + y * ob.y;
        }

        Vector Vector::operator*(float val) {
            return {x * val, y * val};
        }

        float Vector::len() {
            return sqrt(x * x + y * y);
        }

        float Vector::ang() {
            return atan2(y, x);
        }

        Vector Vector::rot90() {
            return {-y, x};
        }

        Vector Vector::norm() {
            float len_ = len();
            if (len_ == 0)
                return {0, 0};
            return {x / len_, y / len_};
        }

        Vector Vector::rot(float a) {
            return {cos(a) * x - sin(a) * y, sin(a) * x + cos(a) * y};
        }

        // Line
        Line::Line(Vector a, Vector b) : a(a), b(b) {}

    }
}

