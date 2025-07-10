#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include "Font.hpp"

class FontManager {
    public:
        static std::shared_ptr<Font> load(const std::string& name, const std::filesystem::path& filepath);
        static std::shared_ptr<Font> get(const std::string& name);
        static void unload(const std::string& name);
        static void unloadAll();

    private:
        static std::unordered_map<std::string, std::shared_ptr<Font>>& fonts();
};