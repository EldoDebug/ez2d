#pragma once
#include "api/Point.hpp"
#include "api/Rect.hpp"
#include <SDL3/SDL.h>
#include <unordered_set>

class Mouse {    
    public:
        static const int Left = SDL_BUTTON_LEFT;
        static const int Middle = SDL_BUTTON_MIDDLE;
        static const int Right = SDL_BUTTON_RIGHT;
        static const int X1 = SDL_BUTTON_X1;
        static const int X2 = SDL_BUTTON_X2;

        static bool isDown(int button);
        static bool isPressed(int button);
        static bool isReleased(int button);
        static void onDown(int button);
        static void onUp(int button);
        static void reset();
        static float getX();
        static float getY();
        static void setPosition(Point point);
        static float getScrollX();
        static float getScrollY();

        static void _updateScroll(Point point) {
            scrollPoint += point;
        }

        static bool isInside(Rect rect);
        
        private:
            static std::unordered_set<int> pressedButtons;
            static std::unordered_set<int> pressedThisFrame;
            static std::unordered_set<int> releasedThisFrame;

            static Point mousePoint;
            static Point scrollPoint;
};
