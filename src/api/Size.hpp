#pragma once

#include "Point.hpp"

class Rect;

class Size {

    public:
        float width, height;

        Size(float width = 0.0f, float height = 0.0f) : width(width), height(height) {}
        Size(const Size& other) : width(other.width), height(other.height) {}

        float getArea() const {
            return width * height;
        }

        float getAspectRatio() const {
            return (height != 0.0f) ? (width / height) : 0.0f;
        }

        bool isEmpty() const {
            return width == 0.0f && height == 0.0f;
        }

        Size operator+(const Size& other) const {
            return Size(width + other.width, height + other.height);
        }

        Size& operator+=(const Size& other) {
            width += other.width;
            height += other.height;
            return *this;
        }

        Size operator-(const Size& other) const {
            return Size(width - other.width, height - other.height);
        }

        Size& operator-=(const Size& other) {
            width -= other.width;
            height -= other.height;
            return *this;
        }

        Size operator*(float scalar) const {
            return Size(width * scalar, height * scalar);
        }

        Size& operator*=(float scalar) {
            width *= scalar;
            height *= scalar;
            return *this;
        }

        Size operator/(float scalar) const {
            return Size(width / scalar, height / scalar);
        }

        Size& operator/=(float scalar) {
            width /= scalar;
            height /= scalar;
            return *this;
        }

        bool operator==(const Size& other) const {
            return width == other.width && height == other.height;
        }

        bool operator!=(const Size& other) const {
            return !(*this == other);
        }

        Size operator-() const {
            return Size(-width, -height);
        }

        Size& operator=(const Size& other) {
            if (this != &other) {
                width = other.width;
                height = other.height;
            }
            return *this;
        }

        Size& operator=(Size&& other) noexcept {
            if (this != &other) {
                width = other.width;
                height = other.height;
                other.width = 0.0f;
                other.height = 0.0f;
            }
            return *this;
        }

        Rect toRect(Point point = Point(0.0f, 0.0f)) const;
};