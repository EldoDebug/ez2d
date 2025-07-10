#include "AudioManager.hpp"
#include "Audio.hpp"
#include <cassert>
#include <memory>
#include <unordered_map>
#include <filesystem>

Audio::Backend AudioManager::backend_ = Audio::Backend::Default;

std::unordered_map<std::string, std::shared_ptr<Audio>>& AudioManager::audios() {
    static std::unordered_map<std::string, std::shared_ptr<Audio>> m_audios;
    return m_audios;
}

void AudioManager::setBackend(Audio::Backend backend) {
    if (backend_ == backend) return;
    backend_ = backend;
    auto& m_audios = audios();
    for (auto& kv : m_audios) {
        kv.second->unload();
    }
    m_audios.clear();
}

void AudioManager::setDefaultBackend(Audio::Backend backend) {
    backend_ = backend;
}

Audio::Backend AudioManager::getBackend() {
    return backend_;
}

std::shared_ptr<Audio> AudioManager::load(const std::string& name, const std::filesystem::path& filepath) {
    auto& m_audios = audios();
    auto it = m_audios.find(name);
    if (it != m_audios.end()) return it->second;
    auto audio = std::make_shared<Audio>(filepath.string(), backend_);
    audio->load();
    m_audios[name] = audio;
    return audio;
}

std::shared_ptr<Audio> AudioManager::get(const std::string& name) {
    auto& m_audios = audios();
    auto it = m_audios.find(name);
    if (it != m_audios.end()) return it->second;
    return nullptr;
}

void AudioManager::clearGarbage() {
    auto& m_audios = audios();
    for (auto it = m_audios.begin(); it != m_audios.end();) {
        if (it->second.use_count() == 1) {
            it->second->unload();
            it = m_audios.erase(it);
        } else {
            ++it;
        }
    }
}

void AudioManager::play(const std::string& name, int startFrame) {
    auto& m_audios = audios();
    auto it = m_audios.find(name);
    if (it != m_audios.end()) {
        it->second->play(startFrame);
    }
}

void AudioManager::playOneShot(const std::string& name, int startFrame) {
    auto& m_audios = audios();
    auto it = m_audios.find(name);
    if (it != m_audios.end()) {
        it->second->playOneShot(startFrame);
    }
}

void AudioManager::stop(const std::string& name) {
    auto& m_audios = audios();
    auto it = m_audios.find(name);
    if (it != m_audios.end()) {
        it->second->stop();
    }
}

void AudioManager::unload(const std::string& name) {
    auto& m_audios = audios();
    auto it = m_audios.find(name);
    if (it != m_audios.end()) {
        it->second->unload();
        m_audios.erase(it);
    }
}

void AudioManager::stopAll() {
    auto& m_audios = audios();
    for (auto& kv : m_audios) {
        kv.second->stop();
    }
}