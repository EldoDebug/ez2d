#include "ControllerManager.hpp"
#include "Controller.hpp"
#include <vector>
#include <string>
#include <SDL3/SDL.h>

std::vector<ControllerInfo> ControllerManager::getConnectedControllers() {
    std::vector<ControllerInfo> result;
    int count;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);
    
    if (joysticks) {
        for (int i = 0; i < count; ++i) {
            if (Controller::isConnected(joysticks[i])) {
                ControllerInfo info;
                info.id = joysticks[i];
                info.connected = true;
                
                SDL_Joystick* joystick = SDL_OpenJoystick(joysticks[i]);
                if (joystick) {
                    const char* name = SDL_GetJoystickName(joystick);
                    info.name = name ? name : "Unknown";
                    info.numButtons = SDL_GetNumJoystickButtons(joystick);
                    info.numAxes = SDL_GetNumJoystickAxes(joystick);
                    SDL_CloseJoystick(joystick);
                } else {
                    info.name = "Unknown";
                    info.numButtons = 0;
                    info.numAxes = 0;
                }
                
                result.push_back(info);
            }
        }
        SDL_free(joysticks);
    }
    return result;
}

bool ControllerManager::anyConnected() {
    int count;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);
    
    if (joysticks) {
        for (int i = 0; i < count; ++i) {
            if (Controller::isConnected(joysticks[i])) {
                SDL_free(joysticks);
                return true;
            }
        }
        SDL_free(joysticks);
    }
    return false;
}