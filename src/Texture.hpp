#pragma once

#include "api/Color.hpp"
#include "api/Size.hpp"
#include <filesystem>
#include <vector>

class Texture {
    
    public:
        int handle = -1;
        std::filesystem::path path;
        Size size = Size(0.0f, 0.0f);
        
        Texture() = default;
        
        Texture(int handle, std::filesystem::path path, Size size)
         : handle(handle), path(path), size(size) {}
        
        Texture(int handle, std::filesystem::path path, Size size, const unsigned char* pixelData, int dataSize)
         : handle(handle), path(path), size(size) {
            if (pixelData && dataSize > 0) {
                pixels.resize(dataSize);
                std::copy(pixelData, pixelData + dataSize, pixels.begin());
            }
        }
        
        const unsigned char* getPixelData() const;
        Color getPixelColor(int x, int y) const;
        
        int getWidth() const { return static_cast<int>(size.width); }
        int getHeight() const { return static_cast<int>(size.height); }
        
    private:
        std::vector<unsigned char> pixels;
};