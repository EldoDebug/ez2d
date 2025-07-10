#include "Point.hpp"
#include "Rect.hpp"

Rect Point::toRect(float width, float height) const {
    return Rect(static_cast<float>(x), static_cast<float>(y), width, height);
}

bool Point::isInside(Rect rect) const {
    return x >= rect.x && x <= rect.x + rect.width &&
        y >= rect.y && y <= rect.y + rect.height;
}