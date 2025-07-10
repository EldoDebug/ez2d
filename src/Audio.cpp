#include "Audio.hpp"
#include "../libs/rtaudio/RtAudio.h"
#include <sndfile.h>
#include <filesystem>
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <cstring>
#include <algorithm>

struct AudioImpl {
    std::unique_ptr<RtAudio> rtAudio;
    std::vector<float> audioData;
    std::string filepath;
    bool loaded = false;
    bool playing = false;
    unsigned int sampleRate = 44100;
    unsigned int channels = 2;
    unsigned int bufferFrames = 256;
    size_t currentFrame = 0;
    float volume = 1.0f;
    float pitch = 1.0f;
    float pan = 0.0f;
    bool looping = false;
    
    struct Voice {
        size_t currentFrame = 0;
        bool active = false;
        float volume = 1.0f;
        float pitch = 1.0f;
        float pan = 0.0f;
    };
    std::vector<Voice> voices;
    size_t nextVoiceIndex = 0;
};

struct Audio::Impl : public AudioImpl {};

static RtAudio::Api to_rtaudio_api(Audio::Backend backend) {
    switch (backend) {
        case Audio::Backend::WASAPI:
        case Audio::Backend::WASAPI_SHARED:
            return RtAudio::WINDOWS_WASAPI;
        case Audio::Backend::DirectSound:
            return RtAudio::WINDOWS_DS;
        case Audio::Backend::ASIO:
        case Audio::Backend::ASIO_SHARED:
            return RtAudio::WINDOWS_ASIO;
        case Audio::Backend::PulseAudio:
            return RtAudio::LINUX_PULSE;
        case Audio::Backend::JACK:
        case Audio::Backend::JACK_SHARED:
            return RtAudio::UNIX_JACK;
        case Audio::Backend::ALSA:
        case Audio::Backend::ALSA_SHARED:
            return RtAudio::LINUX_ALSA;
        case Audio::Backend::OSS:
        case Audio::Backend::OSS_SHARED:
            return RtAudio::LINUX_OSS;
        case Audio::Backend::CoreAudio:
        case Audio::Backend::CoreAudio_SHARED:
            return RtAudio::MACOSX_CORE;
        default:
            return RtAudio::UNSPECIFIED;
    }
}

static RtAudioStreamFlags getStreamFlags(Audio::Backend backend) {
    switch (backend) {
        case Audio::Backend::WASAPI:
        case Audio::Backend::ASIO:
        case Audio::Backend::JACK:
        case Audio::Backend::ALSA:
        case Audio::Backend::OSS:
        case Audio::Backend::CoreAudio:
            return RTAUDIO_MINIMIZE_LATENCY | RTAUDIO_HOG_DEVICE;
        case Audio::Backend::WASAPI_SHARED:
        case Audio::Backend::DirectSound:
        case Audio::Backend::ASIO_SHARED:
        case Audio::Backend::PulseAudio:
        case Audio::Backend::JACK_SHARED:
        case Audio::Backend::ALSA_SHARED:
        case Audio::Backend::OSS_SHARED:
        case Audio::Backend::CoreAudio_SHARED:
        default:
            return RTAUDIO_MINIMIZE_LATENCY;
    }
}

int audioCallback(void* outputBuffer, void* inputBuffer,
                  unsigned int nFrames, double streamTime,
                  RtAudioStreamStatus status, void* userData) {

    AudioImpl* impl = static_cast<AudioImpl*>(userData);
    if (!impl || impl->audioData.empty()) {
        std::memset(outputBuffer, 0, nFrames * impl->channels * sizeof(float));
        return 0;
    }

    float* output = static_cast<float*>(outputBuffer);
    size_t totalFrames = impl->audioData.size() / impl->channels;
    
    std::memset(output, 0, nFrames * impl->channels * sizeof(float));
    
    bool anyVoiceActive = false;
    
    if (impl->playing) {
        anyVoiceActive = true;
        for (unsigned int i = 0; i < nFrames; ++i) {
            if (impl->currentFrame >= totalFrames) {
                if (impl->looping) {
                    impl->currentFrame = 0;
                } else {
                    impl->playing = false;
                    break;
                }
            }
            
            for (unsigned int c = 0; c < impl->channels; ++c) {
                float sample = impl->audioData[impl->currentFrame * impl->channels + c] * impl->volume;
                
                if (impl->channels == 2) {
                    if (c == 0) {
                        sample *= (impl->pan <= 0.0f) ? 1.0f : (1.0f - impl->pan);
                    } else {
                        sample *= (impl->pan >= 0.0f) ? 1.0f : (1.0f + impl->pan);
                    }
                }
                
                output[i * impl->channels + c] += sample;
            }
            
            impl->currentFrame++;
        }
    }
    
    for (auto& voice : impl->voices) {
        if (!voice.active) continue;
        
        anyVoiceActive = true;
        for (unsigned int i = 0; i < nFrames; ++i) {
            if (voice.currentFrame >= totalFrames) {
                voice.active = false;
                break;
            }
            
            for (unsigned int c = 0; c < impl->channels; ++c) {
                float sample = impl->audioData[voice.currentFrame * impl->channels + c] * voice.volume;
                
                if (impl->channels == 2) {
                    if (c == 0) {
                        sample *= (voice.pan <= 0.0f) ? 1.0f : (1.0f - voice.pan);
                    } else {
                        sample *= (voice.pan >= 0.0f) ? 1.0f : (1.0f + voice.pan);
                    }
                }
                
                output[i * impl->channels + c] += sample;
            }
            
            voice.currentFrame++;
        }
    }
    
    if (!anyVoiceActive) {
        return 1;
    }
    
    return 0;
}

Audio::Audio(const std::filesystem::path& filepath, Backend backend) : impl(std::make_unique<Impl>()), backend_(backend) {
    impl->filepath = filepath.string();
    impl->rtAudio = std::make_unique<RtAudio>(to_rtaudio_api(backend));
}

Audio::~Audio() {
    unload();
}

bool loadAudioFile(const std::filesystem::path& filepath, std::vector<float>& audioData, 
                   unsigned int& sampleRate, unsigned int& channels) {
    SF_INFO sfinfo;
    std::memset(&sfinfo, 0, sizeof(sfinfo));
    
    SNDFILE* sndfile = sf_open(filepath.string().c_str(), SFM_READ, &sfinfo);
    if (!sndfile) {
        std::cerr << "Error opening audio file: " << sf_strerror(nullptr) << std::endl;
        return false;
    }
    
    sampleRate = static_cast<unsigned int>(sfinfo.samplerate);
    channels = static_cast<unsigned int>(sfinfo.channels);
    
    sf_count_t totalFrames = sfinfo.frames;
    audioData.resize(totalFrames * channels);
    
    sf_count_t framesRead = sf_readf_float(sndfile, audioData.data(), totalFrames);
    if (framesRead != totalFrames) {
        std::cerr << "Warning: Expected " << totalFrames << " frames, but read " << framesRead << std::endl;
        audioData.resize(framesRead * channels);
    }
    
    sf_close(sndfile);
    return true;
}

bool Audio::load() {
    if (impl->loaded) return true;

    std::filesystem::path filePath(impl->filepath);
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "Audio file does not exist: " << impl->filepath << std::endl;
        return false;
    }

    if (!loadAudioFile(filePath, impl->audioData, impl->sampleRate, impl->channels)) {
        std::cerr << "Failed to load audio file: " << impl->filepath << std::endl;
        return false;
    }

    impl->loaded = true;
    return true;
}

void Audio::play(int startFrame) {
    if (!impl->loaded && !load()) return;

    impl->currentFrame = startFrame > 0 ? startFrame : 0;
    impl->playing = true;

    if (!impl->rtAudio->isStreamOpen()) {
        RtAudio::StreamParameters outputParams;
        outputParams.deviceId = impl->rtAudio->getDefaultOutputDevice();
        outputParams.nChannels = impl->channels;

        RtAudio::StreamOptions options;
        options.flags = getStreamFlags(backend_);

        try {
            impl->rtAudio->openStream(&outputParams, nullptr, RTAUDIO_FLOAT32,
                                     impl->sampleRate, &impl->bufferFrames, 
                                     &audioCallback, impl.get(), &options);
        } catch (RtAudioErrorType& e) {
            impl->playing = false;
            return;
        }
    }

    if (!impl->rtAudio->isStreamRunning()) {
        try {
            impl->rtAudio->startStream();
        } catch (RtAudioErrorType& e) {
            impl->playing = false;
        }
    }
}

void Audio::playOneShot(int startFrame) {
    if (!impl->loaded && !load()) return;
    
    auto it = std::find_if(impl->voices.begin(), impl->voices.end(),
                          [](const AudioImpl::Voice& voice) { return !voice.active; });
    
    if (it != impl->voices.end()) {
        it->currentFrame = startFrame > 0 ? startFrame : 0;
        it->active = true;
        it->volume = impl->volume;
        it->pan = impl->pan;
    } else {
        AudioImpl::Voice newVoice;
        newVoice.currentFrame = startFrame > 0 ? startFrame : 0;
        newVoice.active = true;
        newVoice.volume = impl->volume;
        newVoice.pan = impl->pan;
        impl->voices.push_back(newVoice);
    }
    
    if (!impl->rtAudio->isStreamOpen()) {
        RtAudio::StreamParameters outputParams;
        outputParams.deviceId = impl->rtAudio->getDefaultOutputDevice();
        outputParams.nChannels = impl->channels;

        RtAudio::StreamOptions options;
        options.flags = getStreamFlags(backend_);

        try {
            impl->rtAudio->openStream(&outputParams, nullptr, RTAUDIO_FLOAT32,
                                     impl->sampleRate, &impl->bufferFrames, 
                                     &audioCallback, impl.get(), &options);
        } catch (RtAudioErrorType& e) {
            return;
        }
    }

    if (!impl->rtAudio->isStreamRunning()) {
        try {
            impl->rtAudio->startStream();
        } catch (RtAudioErrorType& e) {
            if (!impl->voices.empty() && impl->voices.back().active) {
                impl->voices.back().active = false;
            }
        }
    }
}

void Audio::stop() {
    impl->playing = false;
    if (impl->rtAudio && impl->rtAudio->isStreamRunning()) {
        try {
            impl->rtAudio->stopStream();
        } catch (RtAudioErrorType& e) {
        }
    }
}

void Audio::unload() {
    stop();
    if (impl->rtAudio && impl->rtAudio->isStreamOpen()) {
        impl->rtAudio->closeStream();
    }
    impl->audioData.clear();
    impl->loaded = false;
}

bool Audio::isPlaying() const {
    return impl->playing && impl->rtAudio && impl->rtAudio->isStreamRunning();
}

uint64_t Audio::getLengthInFrames() const {
    if (impl->loaded && impl->channels > 0) {
        return impl->audioData.size() / impl->channels;
    }
    return 0;
}

uint64_t Audio::getCurrentFrame() const {
    return impl->currentFrame;
}

void Audio::setVolume(float volume) {
    impl->volume = std::max(0.0f, volume);
}

float Audio::getVolume() const {
    return impl->volume;
}

void Audio::setPitch(float pitch) {
    impl->pitch = std::max(0.1f, pitch);
}

float Audio::getPitch() const {
    return impl->pitch;
}

void Audio::setPan(float pan) {
    impl->pan = std::max(-1.0f, std::min(1.0f, pan));
}

float Audio::getPan() const {
    return impl->pan;
}

unsigned Audio::getSampleRate() const {
    return impl->sampleRate;
}

unsigned Audio::getBufferFrames() const {
    return impl->bufferFrames;
}

double Audio::getBufferLatencySeconds() const {
    return double(impl->bufferFrames) / double(impl->sampleRate);
}

double Audio::getTimeSeconds() const {
    return double(impl->currentFrame) / double(impl->sampleRate);
}

std::vector<Audio::Backend> Audio::getAvailableBackends() {
    std::vector<RtAudio::Api> compiledApis;
    RtAudio::getCompiledApi(compiledApis);
    
    std::vector<Audio::Backend> backends;
    for (auto api : compiledApis) {
        switch (api) {
            case RtAudio::WINDOWS_WASAPI:
                backends.push_back(Backend::WASAPI);
                backends.push_back(Backend::WASAPI_SHARED);
                break;
            case RtAudio::WINDOWS_DS:
                backends.push_back(Backend::DirectSound);
                break;
            case RtAudio::WINDOWS_ASIO:
                backends.push_back(Backend::ASIO);
                backends.push_back(Backend::ASIO_SHARED);
                break;
            case RtAudio::LINUX_PULSE:
                backends.push_back(Backend::PulseAudio);
                break;
            case RtAudio::UNIX_JACK:
                backends.push_back(Backend::JACK);
                backends.push_back(Backend::JACK_SHARED);
                break;
            case RtAudio::LINUX_ALSA:
                backends.push_back(Backend::ALSA);
                backends.push_back(Backend::ALSA_SHARED);
                break;
            case RtAudio::LINUX_OSS:
                backends.push_back(Backend::OSS);
                backends.push_back(Backend::OSS_SHARED);
                break;
            case RtAudio::MACOSX_CORE:
                backends.push_back(Backend::CoreAudio);
                backends.push_back(Backend::CoreAudio_SHARED);
                break;
            case RtAudio::UNSPECIFIED:
                backends.push_back(Backend::Default);
                break;
            default:
                break;
        }
    }
    
    if (backends.empty()) {
        backends.push_back(Backend::Default);
    }
    
    return backends;
}