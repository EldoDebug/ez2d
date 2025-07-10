#pragma once
#include <vector>
#include <string>
#include <SDL3/SDL.h>

struct ControllerInfo {
    int id;
    std::string name;
    bool connected;
    int numButtons;
    int numAxes;
};

class ControllerManager {
    public:
        static std::vector<ControllerInfo> getConnectedControllers();
        static bool anyConnected();
};