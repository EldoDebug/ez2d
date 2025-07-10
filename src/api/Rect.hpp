#pragma once
#include "Size.hpp"
#include "Point.hpp"

class Rect {

    public:
        float x, y, width, height;

        Rect(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f)
            : x(x), y(y), width(width), height(height) {}
        Rect(const Point& point, const Size& size)
            : x(point.x), y(point.y), width(size.width), height(size.height) {}
        Rect(float x, float y, const Size& size)
            : x(x), y(y), width(size.width), height(size.height) {}
        Rect(Point point, float width, float height)
            : x(point.x), y(point.y), width(width), height(height) {}
        Rect(const Rect& other)
            : x(other.x), y(other.y), width(other.width), height(other.height) {}

        Point getCenter() const;
        Point getTopLeft() const;
        Point getTopRight() const;
        Point getBottomLeft() const;
        Point getBottomRight() const;
        
        float getArea() const { return width * height; }
        float getAspectRatio() const { return (height != 0.0f) ? (width / height) : 0.0f; }

        bool isEmpty() const { return width <= 0.0f || height <= 0.0f; }
        bool isValid() const { return width >= 0.0f && height >= 0.0f; }

        Size toSize() const;
        Point toPoint() const;
};