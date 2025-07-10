#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include "Sprite.hpp"

class SpriteManager {
    public:
        static std::shared_ptr<Sprite> load(const std::string& name, const std::filesystem::path& filepath, int width, int height);
        static std::shared_ptr<Sprite> get(const std::string& name);

        static void clearGarbage();
        static void unload(const std::string& name);
        static void unloadAll();
        
    private:
        static std::unordered_map<std::string, std::shared_ptr<Sprite>>& sprites();
};