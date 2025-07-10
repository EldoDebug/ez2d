#pragma once

#include "Element.hpp"
#include "../Point.hpp"
#include <memory>

class RootElement : public Element, public std::enable_shared_from_this<RootElement> {

    public:
        RootElement(float screenWidth = 0, float screenHeight = 0);
        
        void resize(float width, float height);
        void updateLayout();

        void draw() override;
        
    private:
        float screenWidth;
        float screenHeight;
        Point lastMousePosition;
};
