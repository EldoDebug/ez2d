#pragma once
#include "Audio.hpp"
#include "Example.hpp"
#include "Scene.hpp"
#include <memory>

class AudioExample : public Example {

    private:
        class AudioScene : public Scene {

            private:
                std::shared_ptr<Audio> audio;

            public:
                void onInit() override;
                void onUpdate() override;
        };

    public:
        void run() override;
};