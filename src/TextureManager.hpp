#include "Texture.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

class TextureManager {

    public:
        static std::shared_ptr<Texture> load(std::string name, std::filesystem::path path);
        static std::shared_ptr<Texture> get(std::string name);
        static void unload(std::string name);
        static void unloadAll();
        void clearGarbage();

    private:
        static std::unordered_map<std::string, std::shared_ptr<Texture>>& textures();
};