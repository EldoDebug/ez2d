#pragma once
#include "Example.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "SpriteAnimation.hpp"
#include <memory>

class SpriteAnimationExample : public Example {

    private:
        class SpriteAnimationScene : public Scene {

            private:
                std::shared_ptr<Sprite> sprite;
                std::shared_ptr<SpriteAnimation> spriteAnimation;

            public:
                void onInit() override;
                void onRender() override;
                void onUpdate() override;
        };

    public:
        void run() override;
};