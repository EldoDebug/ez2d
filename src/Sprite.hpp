#pragma once

#include "Texture.hpp"
#include <memory>

class Sprite {
    public:
        std::shared_ptr<Texture> texture;
        Size size = Size(0.0f, 0.0f);
        
        Sprite() = default;
        
        Sprite(std::shared_ptr<Texture> texture, Size size)
         : texture(texture), size(size) {}
};