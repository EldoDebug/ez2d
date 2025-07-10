#include "Texture.hpp"

const unsigned char* Texture::getPixelData() const {
    return pixels.empty() ? nullptr : pixels.data();
}

Color Texture::getPixelColor(int x, int y) const {
    
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight() || pixels.empty()) {
        return Color(0, 0, 0, 0);
    }
    
    int index = (y * getWidth() + x) * 4;

    if (index + 3 >= static_cast<int>(pixels.size())) {
        return Color(0, 0, 0, 0);
    }
    
    return Color(pixels[index], pixels[index + 1], pixels[index + 2], pixels[index + 3]);
}