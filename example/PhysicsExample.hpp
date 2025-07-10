#pragma once
#include "Example.hpp"
#include "Scene.hpp"
#include "api/World.hpp"
#include <memory>

class PhysicsExample : public Example {

    private:
        class PhysicsScene : public Scene {

            private:
                std::unique_ptr<World> world;
                std::shared_ptr<Object> ground;

            public:
                void onInit() override;
                void onRender() override;
                void onUpdate() override;
                void onMousePressed(Point point, int button) override;
        };

    public:
        void run() override;
};