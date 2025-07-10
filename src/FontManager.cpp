#include "FontManager.hpp"
#include "Renderer.hpp"
#include <memory>
#include <unordered_map>
#include <filesystem>

std::unordered_map<std::string, std::shared_ptr<Font>>& FontManager::fonts() {
    static std::unordered_map<std::string, std::shared_ptr<Font>> m_fonts;
    return m_fonts;
}

std::shared_ptr<Font> FontManager::load(const std::string& name, const std::filesystem::path& filepath) {
    auto& m_fonts = fonts();
    auto it = m_fonts.find(name);
    if (it != m_fonts.end()) return it->second;

    int handle = nvgCreateFont(Renderer::context, name.c_str(), filepath.string().c_str());
    if (handle == -1) return nullptr;

    auto font = std::make_shared<Font>(filepath.string(), handle);
    m_fonts[name] = font;
    return font;
}

std::shared_ptr<Font> FontManager::get(const std::string& name) {
    auto& m_fonts = fonts();
    auto it = m_fonts.find(name);
    if (it != m_fonts.end()) return it->second;
    return nullptr;
}

void FontManager::unload(const std::string& name) {
    auto& m_fonts = fonts();
    auto it = m_fonts.find(name);
    if (it != m_fonts.end()) {
        m_fonts.erase(it);
    }
}

void FontManager::unloadAll() {
    auto& m_fonts = fonts();
    m_fonts.clear();
}