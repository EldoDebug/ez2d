#pragma once
#include "Example.hpp"
#include "Font.hpp"
#include "Scene.hpp"
#include <memory>

class FontExample : public Example {

    private:
        class FontScene : public Scene {

            private:
                std::shared_ptr<Font> font;

            public:
                void onInit() override;
                void onRender() override;
        };

    public:
        void run() override;
};