#pragma once

class Vector2;
class Rect;

class Point {

    public:
        float x, y;

        Point(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
        Point(const Point& other) : x(other.x), y(other.y) {}

        Vector2 toVector2() const;
        Rect toRect(float width = 0.0f, float height = 0.0f) const;

        Point operator+(const Point& other) const {
            return Point(x + other.x, y + other.y);
        }

        Point& operator+=(const Point& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Point operator-(const Point& other) const {
            return Point(x - other.x, y - other.y);
        }

        Point& operator-=(const Point& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Point operator*(float scalar) const {
            return Point(x * scalar, y * scalar);
        }

        Point& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Point operator/(float scalar) const {
            return Point(x / scalar, y / scalar);
        }

        Point& operator/=(float scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Point& other) const {
            return !(*this == other);
        }

        Point operator-() const {
            return Point(-x, -y);
        }

        Point& operator=(const Point& other) {
            if (this != &other) {
                x = other.x;
                y = other.y;
            }
            return *this;
        }

        Point& operator=(Point&& other) noexcept {
            if (this != &other) {
                x = other.x;
                y = other.y;
                other.x = 0.0f;
                other.y = 0.0f;
            }
            return *this;
        }

        bool isInside(Rect rect) const;
};