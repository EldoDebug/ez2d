#include "TextureManager.hpp"
#include "Renderer.hpp"
#include "stb_image.h"
#include <memory>

std::unordered_map<std::string, std::shared_ptr<Texture>>& TextureManager::textures() {
    static std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
    return m_textures;
}

std::shared_ptr<Texture> TextureManager::load(std::string name, std::filesystem::path path) {

    auto& m_textures = textures();
    auto it = m_textures.find(name);
    if (it != m_textures.end()) return it->second;

    int w, h, n;
    unsigned char* data = stbi_load(path.string().c_str(), &w, &h, &n, 4);
    
    if (!data) {
        return nullptr;
    }

    int image = nvgCreateImageRGBA(Renderer::context, w, h, 0, data);
    
    if (image == 0) {
        stbi_image_free(data);
        return nullptr;
    }

    auto tex = std::make_shared<Texture>(image, path.string(), Size(static_cast<float>(w), static_cast<float>(h)), data, w * h * 4);
    stbi_image_free(data);
    m_textures[name] = tex;
    return tex;
}

std::shared_ptr<Texture> TextureManager::get(std::string name) {
    auto& m_textures = textures();
    auto it = m_textures.find(name);
    if (it != m_textures.end()) return it->second;
    return nullptr;
}

void TextureManager::clearGarbage() {
    auto& m_textures = textures();
    for (auto it = m_textures.begin(); it != m_textures.end();) {
        if (it->second.use_count() == 1) {
            nvgDeleteImage(Renderer::context, it->second->handle);
            it = m_textures.erase(it);
        } else {
            ++it;
        }
    }
}

void TextureManager::unload(std::string name) {
    auto& m_textures = textures();
    auto it = m_textures.find(name);
    if (it != m_textures.end()) {
        nvgDeleteImage(Renderer::context, it->second->handle);
        m_textures.erase(it);
    }
}

void TextureManager::unloadAll() {
    
    auto& m_textures = textures();

    for (auto& kv : m_textures) {
        nvgDeleteImage(Renderer::context, kv.second->handle);
    }
    
    m_textures.clear();
}