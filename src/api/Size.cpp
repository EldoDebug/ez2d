#include "Size.hpp"
#include "Rect.hpp"

Rect Size::toRect(Point point) const {
    return Rect(point.x, point.y, width, height);
}