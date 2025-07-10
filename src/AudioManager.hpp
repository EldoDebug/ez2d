#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include "Audio.hpp"

class AudioManager {
    public:
        static void setDefaultBackend(Audio::Backend backend);
        static void setBackend(Audio::Backend backend);
        static Audio::Backend getBackend();
        static std::shared_ptr<Audio> load(const std::string& name, const std::filesystem::path& filepath);
        static std::shared_ptr<Audio> get(const std::string& name);
        static void clearGarbage();
        static void play(const std::string& name, int startFrame = 0);
        static void playOneShot(const std::string& name, int startFrame = 0);
        static void stop(const std::string& name);
        static void unload(const std::string& name);
        static void stopAll();
    
    private:
        static std::unordered_map<std::string, std::shared_ptr<Audio>>& audios();
        static Audio::Backend backend_;
};