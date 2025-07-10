#pragma once
#include "nanovg.h"

class Color {

    public:
        int red;
        int green;
        int blue;
        int alpha;

        Color() : red(0), green(0), blue(0), alpha(255) {}
        Color(int red, int green, int blue) : red(red), green(green), blue(blue), alpha(255) {}
        Color(int red, int green, int blue, int alpha) : red(red), green(green), blue(blue), alpha(alpha) {}

        static Color fromHSV(float hue, float saturation, float value, int alpha = 255);
        
        Color lerp(const Color& other, float t) const;

        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;
        static const Color Transparent;

        NVGcolor toNVGColor();
        
        Color applyAlpha(int alpha) const;
        float getBrightness() const;
};