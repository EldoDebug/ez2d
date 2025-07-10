#pragma once
#include "Example.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include <memory>

class TextureExample : public Example {

    private:
        class TextureScene : public Scene {

            private:
                std::shared_ptr<Texture> texture;

            public:
                void onInit() override;
                void onRender() override;
                void onExit() override;
        };

    public:
        void run() override;
};
