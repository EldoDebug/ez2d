#pragma once
#include "Animation.hpp"
#include "Example.hpp"
#include "Scene.hpp"

class AnimationExample : public Example {

    private:
        class AnimationScene : public Scene {

            private:
                Animation animation;

            public:
                void onInit() override;
                void onRender() override;
                void onUpdate() override;
        };

    public:
        void run() override;
};