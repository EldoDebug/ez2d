#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <atomic>
#include <thread>

class ResourceLoader {
public:
    ResourceLoader();
    ~ResourceLoader();

    void addTexture(const std::string& name, const std::filesystem::path& path);
    void addFont(const std::string& name, const std::filesystem::path& path);
    void addSprite(const std::string& name, const std::filesystem::path& path, int width, int height);
    void addAudio(const std::string& name, const std::filesystem::path& path);

    void start(size_t threadCount = 1);
    void wait();
    float getProgress() const;
    bool isFinished() const;

private:
    enum class Type {
        Texture, Font, Sprite, Audio
    };
    struct Task {
        Type type;
        std::string name;
        std::filesystem::path path;
        int width;
        int height;
    };

    std::vector<Task> tasks_;
    std::atomic<size_t> completed_;
    std::atomic<bool> started_;
    std::vector<std::thread> workers_;

    void run();
    void runRange(size_t begin, size_t end);
};
