#pragma once
#include <SDL3/SDL.h>

class Controller {
    public:
        static bool isConnected(int id = 0);
        static bool isPressed(int button, int id = 0);
        static float getAxis(int axis, int id = 0);
};