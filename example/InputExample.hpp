#include "Example.hpp"
#include "Scene.hpp"
#include "api/Color.hpp"

class InputExample : public Example {

    private:
        class InputScene : public Scene {

            private:
                float x = 0.0F;
                float y = 0.0F;
                Color circleColor = Color::Red;

            public:
                void onRender() override;
                void onUpdate() override;
        };

    public:
        void run() override;
};