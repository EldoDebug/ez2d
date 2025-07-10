#pragma once
#include <memory>
#include <vector>
#include <filesystem>

class Audio {
    public:
        enum class Backend {
            WASAPI,
            WASAPI_SHARED,
            DirectSound,
            ASIO,
            ASIO_SHARED,
            PulseAudio,
            JACK,
            JACK_SHARED,
            ALSA,
            ALSA_SHARED,
            OSS,
            OSS_SHARED,
            CoreAudio,
            CoreAudio_SHARED,
            Default
        };

        Audio(const std::filesystem::path& filepath, Backend backend = Backend::Default);
        ~Audio();

        bool load();
        void play(int startFrame = 0);
        void playOneShot(int startFrame = 0);
        void stop();
        void unload();
        bool isPlaying() const;
        
        uint64_t getLengthInFrames() const;
        uint64_t getCurrentFrame() const;
        unsigned getBufferFrames() const;
        double getBufferLatencySeconds() const;
        unsigned getSampleRate() const;
        double getTimeSeconds() const;
        
        void setVolume(float volume);
        float getVolume() const;
        void setPitch(float pitch);
        float getPitch() const;
        void setPan(float pan);
        float getPan() const;
        
        static std::vector<Backend> getAvailableBackends();
    
    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
        Backend backend_ = Backend::Default;
};