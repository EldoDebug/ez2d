#pragma once
#include <string>

class Font {

    public:
        std::string path;
        int handle;
        
    public:
        Font() = default;
        Font(std::string path, int handle) : path(path), handle(handle) {}
};