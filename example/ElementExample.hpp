#pragma once
#include "Example.hpp"
#include "Scene.hpp"
#include <memory>
#include "api/element/RootElement.hpp"

class ElementExample : public Example {

    private:
        class ElementScene : public Scene {

            private:
                std::shared_ptr<RootElement> rootElement;

            public:
                void onInit() override;
                void onUpdate() override;
                void onRender() override;
                void onResize(int width, int height) override;
        };

    public:
        void run() override;
};