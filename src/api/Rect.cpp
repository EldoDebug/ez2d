#include "Rect.hpp"
#include "Size.hpp"
#include "Point.hpp"

Size Rect::toSize() const {
    return Size(width, height);
}

Point Rect::toPoint() const {
    return Point(x, y);
}

Point Rect::getCenter() const {
    return Point(x + width * 0.5f, y + height * 0.5f);
}

Point Rect::getTopLeft() const {
    return Point(x, y);
}

Point Rect::getTopRight() const {
    return Point(x + width, y);
}

Point Rect::getBottomLeft() const {
    return Point(x, y + height);
}

Point Rect::getBottomRight() const {
    return Point(x + width, y + height);
}