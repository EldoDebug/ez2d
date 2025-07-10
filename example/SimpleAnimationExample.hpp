#pragma once
#include "Example.hpp"
#include "Scene.hpp"
#include "SimpleAnimation.hpp"

class SimpleAnimationExample : public Example {

    private:
        class AnimationScene : public Scene {

            private:
                float y = 0.0F;
                SimpleAnimation animation;

            public:
                void onRender() override;
                void onUpdate() override;
        };

    public:
        void run() override;
};