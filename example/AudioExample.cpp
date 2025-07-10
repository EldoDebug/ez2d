#include "AudioExample.hpp"
#include "Audio.hpp"
#include "AudioManager.hpp"
#include "Keyboard.hpp"
#include "Window.hpp"

void AudioExample::run() {

    Window::Config config;

    config.title = "Audio Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<AudioExample::AudioScene>();
    Window::show();
    Window::shutdown();
}

void AudioExample::AudioScene::onInit() {
    audio = AudioManager::load("test", "path/to/your/audio/file.wav");
}

void AudioExample::AudioScene::onUpdate() {
    if(Keyboard::isPressed(Keyboard::Space)) {
        if(audio) {
            audio->play();
        }
    }
}