#pragma once
#include "Example.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include <memory>

class SpriteExample : public Example {

    private:
        class SpriteScene : public Scene {

            private:
                std::shared_ptr<Sprite> sprite;

            public:
                void onInit() override;
                void onRender() override;
                void onUpdate() override;
        };

    public:
        void run() override;
};