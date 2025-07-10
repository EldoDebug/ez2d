#include "ResourceLoader.hpp"
#include <vector>
#include "AudioManager.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "SpriteManager.hpp"
#include <stb_image.h>

ResourceLoader::ResourceLoader()
: completed_(0), started_(false) {}

ResourceLoader::~ResourceLoader() {
    for (auto& w : workers_) {
        if (w.joinable()) w.join();
    }
}

void ResourceLoader::addTexture(const std::string& name, const std::filesystem::path& path) {
    tasks_.push_back({Type::Texture, name, path, 0, 0});
}

void ResourceLoader::addFont(const std::string& name, const std::filesystem::path& path) {
    tasks_.push_back({Type::Font, name, path, 0, 0});
}

void ResourceLoader::addSprite(const std::string& name, const std::filesystem::path& path, int width, int height) {
    tasks_.push_back({Type::Sprite, name, path, width, height});
}

void ResourceLoader::addAudio(const std::string& name, const std::filesystem::path& path) {
    tasks_.push_back({Type::Audio, name, path, 0, 0});
}

void ResourceLoader::start(size_t threadCount) {
    if (started_) return;
    started_ = true;
    size_t total = tasks_.size();
    if (threadCount <= 1 || total < threadCount) {
        workers_.emplace_back(&ResourceLoader::run, this);
    } else {
        size_t chunk = total / threadCount;
        for (size_t i = 0; i < threadCount; ++i) {
            size_t begin = i * chunk;
            size_t end = (i == threadCount - 1) ? total : begin + chunk;
            workers_.emplace_back(&ResourceLoader::runRange, this, begin, end);
        }
    }
}

void ResourceLoader::wait() {
    for (auto& w : workers_) {
        if (w.joinable()) w.join();
    }
}

float ResourceLoader::getProgress() const {
    if (tasks_.empty()) return 1.0f;
    return static_cast<float>(completed_) / static_cast<float>(tasks_.size());
}

bool ResourceLoader::isFinished() const {
    return started_ && completed_ == tasks_.size();
}

void ResourceLoader::run() {
    runRange(0, tasks_.size());
}

void ResourceLoader::runRange(size_t begin, size_t end) {
    for (size_t i = begin; i < end; ++i) {
        const auto& t = tasks_[i];
        switch (t.type) {
            case Type::Texture: {
                TextureManager::load(t.name, t.path);
                break;
            }
            case Type::Font: {
                FontManager::load(t.name, t.path);
                break;
            }
            case Type::Sprite: {
                SpriteManager::load(t.name, t.path, t.width, t.height);
                break;
            }
            case Type::Audio: {
                AudioManager::load(t.name, t.path);
                break;
            }
        }
        ++completed_;
    }
}
