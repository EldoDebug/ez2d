#pragma once
#include "Example.hpp"
#include "Font.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "ResourceLoader.hpp"
#include <memory>

class ResourceLoaderExample : public Example {

    private:
        class ResourceLoaderScene : public Scene {

            private:
                ResourceLoader loader;
                std::shared_ptr<Texture> texture;
                std::shared_ptr<Texture> texture2;
                std::shared_ptr<Font> font;

            public:
                void onInit() override;
                void onRender() override;
        };

    public:
        void run() override;
};