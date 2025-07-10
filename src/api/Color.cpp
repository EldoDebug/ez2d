#include "Color.hpp"
#include "nanovg.h"
#include <algorithm>
#include <cmath>

NVGcolor Color::toNVGColor() {
    return nvgRGBA(red, green, blue, alpha);
}

Color Color::fromHSV(float hue, float saturation, float value, int alpha) {
    
    float c = value * saturation;
    float x = c * (1 - std::abs(std::fmod(hue / 60.0f, 2) - 1));
    float m = value - c;
    
    float r, g, b;
    if (hue >= 0 && hue < 60) {
        r = c; g = x; b = 0;
    } else if (hue >= 60 && hue < 120) {
        r = x; g = c; b = 0;
    } else if (hue >= 120 && hue < 180) {
        r = 0; g = c; b = x;
    } else if (hue >= 180 && hue < 240) {
        r = 0; g = x; b = c;
    } else if (hue >= 240 && hue < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }
    
    return Color(
        static_cast<int>((r + m) * 255),
        static_cast<int>((g + m) * 255),
        static_cast<int>((b + m) * 255),
        alpha
    );
}

Color Color::lerp(const Color& other, float t) const {
    t = std::clamp(t, 0.0f, 1.0f);
    return Color(
        static_cast<int>(red + t * (other.red - red)),
        static_cast<int>(green + t * (other.green - green)),
        static_cast<int>(blue + t * (other.blue - blue)),
        static_cast<int>(alpha + t * (other.alpha - alpha))
    );
}

Color Color::applyAlpha(int alpha) const {
    return Color(red, green, blue, alpha);
}

float Color::getBrightness() const {
    return (red * 0.299f + green * 0.587f + blue * 0.114f) / 255.0f;
}

const Color Color::White(255, 255, 255);
const Color Color::Black(0, 0, 0);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Cyan(0, 255, 255);
const Color Color::Magenta(255, 0, 255);
const Color Color::Transparent(0, 0, 0, 0);