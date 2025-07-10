#pragma once

#include <SDL3/SDL.h>
#include <unordered_set>

class Keyboard {
    public:
        static const SDL_Keycode Unknown = SDLK_UNKNOWN;
        static const SDL_Keycode Return = SDLK_RETURN;
        static const SDL_Keycode Enter = SDLK_RETURN;
        static const SDL_Keycode Escape = SDLK_ESCAPE;
        static const SDL_Keycode Backspace = SDLK_BACKSPACE;
        static const SDL_Keycode Tab = SDLK_TAB;
        static const SDL_Keycode Space = SDLK_SPACE;
        static const SDL_Keycode Delete = SDLK_DELETE;
        
        static const SDL_Keycode Num0 = SDLK_0;
        static const SDL_Keycode Num1 = SDLK_1;
        static const SDL_Keycode Num2 = SDLK_2;
        static const SDL_Keycode Num3 = SDLK_3;
        static const SDL_Keycode Num4 = SDLK_4;
        static const SDL_Keycode Num5 = SDLK_5;
        static const SDL_Keycode Num6 = SDLK_6;
        static const SDL_Keycode Num7 = SDLK_7;
        static const SDL_Keycode Num8 = SDLK_8;
        static const SDL_Keycode Num9 = SDLK_9;
        
        static const SDL_Keycode A = SDLK_A;
        static const SDL_Keycode B = SDLK_B;
        static const SDL_Keycode C = SDLK_C;
        static const SDL_Keycode D = SDLK_D;
        static const SDL_Keycode E = SDLK_E;
        static const SDL_Keycode F = SDLK_F;
        static const SDL_Keycode G = SDLK_G;
        static const SDL_Keycode H = SDLK_H;
        static const SDL_Keycode I = SDLK_I;
        static const SDL_Keycode J = SDLK_J;
        static const SDL_Keycode K = SDLK_K;
        static const SDL_Keycode L = SDLK_L;
        static const SDL_Keycode M = SDLK_M;
        static const SDL_Keycode N = SDLK_N;
        static const SDL_Keycode O = SDLK_O;
        static const SDL_Keycode P = SDLK_P;
        static const SDL_Keycode Q = SDLK_Q;
        static const SDL_Keycode R = SDLK_R;
        static const SDL_Keycode S = SDLK_S;
        static const SDL_Keycode T = SDLK_T;
        static const SDL_Keycode U = SDLK_U;
        static const SDL_Keycode V = SDLK_V;
        static const SDL_Keycode W = SDLK_W;
        static const SDL_Keycode X = SDLK_X;
        static const SDL_Keycode Y = SDLK_Y;
        static const SDL_Keycode Z = SDLK_Z;
        
        static const SDL_Keycode Up = SDLK_UP;
        static const SDL_Keycode Down = SDLK_DOWN;
        static const SDL_Keycode Left = SDLK_LEFT;
        static const SDL_Keycode Right = SDLK_RIGHT;
        
        static const SDL_Keycode F1 = SDLK_F1;
        static const SDL_Keycode F2 = SDLK_F2;
        static const SDL_Keycode F3 = SDLK_F3;
        static const SDL_Keycode F4 = SDLK_F4;
        static const SDL_Keycode F5 = SDLK_F5;
        static const SDL_Keycode F6 = SDLK_F6;
        static const SDL_Keycode F7 = SDLK_F7;
        static const SDL_Keycode F8 = SDLK_F8;
        static const SDL_Keycode F9 = SDLK_F9;
        static const SDL_Keycode F10 = SDLK_F10;
        static const SDL_Keycode F11 = SDLK_F11;
        static const SDL_Keycode F12 = SDLK_F12;
        
        static const SDL_Keycode LShift = SDLK_LSHIFT;
        static const SDL_Keycode RShift = SDLK_RSHIFT;
        static const SDL_Keycode LCtrl = SDLK_LCTRL;
        static const SDL_Keycode RCtrl = SDLK_RCTRL;
        static const SDL_Keycode LAlt = SDLK_LALT;
        static const SDL_Keycode RAlt = SDLK_RALT;
        static const SDL_Keycode LGui = SDLK_LGUI;
        static const SDL_Keycode RGui = SDLK_RGUI;
        
        static const SDL_Keycode Capslock = SDLK_CAPSLOCK;
        static const SDL_Keycode Insert = SDLK_INSERT;
        static const SDL_Keycode Home = SDLK_HOME;
        static const SDL_Keycode End = SDLK_END;
        static const SDL_Keycode Pageup = SDLK_PAGEUP;
        static const SDL_Keycode Pagedown = SDLK_PAGEDOWN;
        static const SDL_Keycode Printscreen = SDLK_PRINTSCREEN;
        static const SDL_Keycode Scrolllock = SDLK_SCROLLLOCK;
        static const SDL_Keycode Pause = SDLK_PAUSE;
        
        static const SDL_Keycode Numpad0 = SDLK_KP_0;
        static const SDL_Keycode Numpad1 = SDLK_KP_1;
        static const SDL_Keycode Numpad2 = SDLK_KP_2;
        static const SDL_Keycode Numpad3 = SDLK_KP_3;
        static const SDL_Keycode Numpad4 = SDLK_KP_4;
        static const SDL_Keycode Numpad5 = SDLK_KP_5;
        static const SDL_Keycode Numpad6 = SDLK_KP_6;
        static const SDL_Keycode Numpad7 = SDLK_KP_7;
        static const SDL_Keycode Numpad8 = SDLK_KP_8;
        static const SDL_Keycode Numpad9 = SDLK_KP_9;
        static const SDL_Keycode NumpadEnter = SDLK_KP_ENTER;
        static const SDL_Keycode NumpadPlus = SDLK_KP_PLUS;
        static const SDL_Keycode NumpadMinus = SDLK_KP_MINUS;
        static const SDL_Keycode NumpadMultiply = SDLK_KP_MULTIPLY;
        static const SDL_Keycode NumpadDivide = SDLK_KP_DIVIDE;
        static const SDL_Keycode NumpadPeriod = SDLK_KP_PERIOD;
        
        static const SDL_Keycode Comma = SDLK_COMMA;
        static const SDL_Keycode Period = SDLK_PERIOD;
        static const SDL_Keycode Slash = SDLK_SLASH;
        static const SDL_Keycode Semicolon = SDLK_SEMICOLON;
        static const SDL_Keycode Apostrophe = SDLK_APOSTROPHE;
        static const SDL_Keycode Leftbracket = SDLK_LEFTBRACKET;
        static const SDL_Keycode Rightbracket = SDLK_RIGHTBRACKET;
        static const SDL_Keycode Backslash = SDLK_BACKSLASH;
        static const SDL_Keycode Minus = SDLK_MINUS;
        static const SDL_Keycode Equals = SDLK_EQUALS;
        static const SDL_Keycode Grave = SDLK_GRAVE;
        
        static bool isDown(SDL_Keycode key);
        static bool isPressed(SDL_Keycode key);
        static bool isReleased(SDL_Keycode key);
        static void onDown(SDL_Keycode key);
        static void onUp(SDL_Keycode key);
        static void reset();

    private:
        static std::unordered_set<SDL_Keycode> pressedKeys;
        static std::unordered_set<SDL_Keycode> pressedThisFrame;
        static std::unordered_set<SDL_Keycode> releasedThisFrame;
};
