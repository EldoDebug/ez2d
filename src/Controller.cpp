#include "Controller.hpp"

bool Controller::isConnected(int id) {
    int count;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);
    
    if (joysticks) {
        for (int i = 0; i < count; ++i) {
            if (joysticks[i] == id) {
                SDL_free(joysticks);
                return true;
            }
        }
        SDL_free(joysticks);
    }
    return false;
}

bool Controller::isPressed(int button, int id) {
    SDL_Joystick* joystick = SDL_OpenJoystick(id);
    if (!joystick) return false;
    
    bool pressed = SDL_GetJoystickButton(joystick, button) == 1;
    SDL_CloseJoystick(joystick);
    return pressed;
}

float Controller::getAxis(int axis, int id) {
    SDL_Joystick* joystick = SDL_OpenJoystick(id);
    if (!joystick) return 0.0f;
    
    Sint16 value = SDL_GetJoystickAxis(joystick, axis);
    SDL_CloseJoystick(joystick);
    return value / 32767.0f;
}
