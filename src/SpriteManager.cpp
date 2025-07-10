#include "SpriteManager.hpp"
#include "Sprite.hpp"
#include "api/Size.hpp"
#include "stb_image.h"
#include "Renderer.hpp"
#include <memory>
#include <unordered_map>
#include <filesystem>

std::unordered_map<std::string, std::shared_ptr<Sprite>>& SpriteManager::sprites() {
    static std::unordered_map<std::string, std::shared_ptr<Sprite>> m_sprites;
    return m_sprites;
}

std::shared_ptr<Sprite> SpriteManager::load(const std::string& name, const std::filesystem::path& filepath, int width, int height) {
    auto& m_sprites = sprites();
    auto it = m_sprites.find(name);
    if (it != m_sprites.end()) return it->second;

    int w, h, n;
    unsigned char* data = stbi_load(filepath.string().c_str(), &w, &h, &n, 4);
    if (!data) return nullptr;

    int image = nvgCreateImageRGBA(Renderer::context, w, h, 0, data);
    stbi_image_free(data);
    if (image == 0) return nullptr;

    Texture texture(image, filepath.string(), Size(static_cast<float>(w), static_cast<float>(h)));
    auto sprite = std::make_shared<Sprite>(std::make_shared<Texture>(texture), Size(static_cast<float>(width), static_cast<float>(height)));
    m_sprites[name] = sprite;
    return sprite;
}

std::shared_ptr<Sprite> SpriteManager::get(const std::string& name) {
    auto& m_sprites = sprites();
    auto it = m_sprites.find(name);
    if (it != m_sprites.end()) return it->second;
    return nullptr;
}

void SpriteManager::clearGarbage() {
    auto& m_sprites = sprites();
    for (auto it = m_sprites.begin(); it != m_sprites.end();) {
        if (it->second.use_count() == 1) {
            nvgDeleteImage(Renderer::context, it->second->texture->handle);
            it = m_sprites.erase(it);
        } else {
            ++it;
        }
    }
}

void SpriteManager::unload(const std::string& name) {
    auto& m_sprites = sprites();
    auto it = m_sprites.find(name);
    if (it != m_sprites.end()) {
        nvgDeleteImage(Renderer::context, it->second->texture->handle);
        m_sprites.erase(it);
    }
}

void SpriteManager::unloadAll() {
    
    auto& m_sprites = sprites();

    for (auto& kv : m_sprites) {
        nvgDeleteImage(Renderer::context, kv.second->texture->handle);
    }

    m_sprites.clear();
}