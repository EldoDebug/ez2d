#pragma once

#include "api/Point.hpp"

class Camera {
    
    public:
        Point point = { 0.0F, 0.0F };
        float zoom = 1.0F;
        float angle = 0.0F;

        Camera() = default;
};