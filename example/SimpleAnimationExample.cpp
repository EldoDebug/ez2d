#include "SimpleAnimationExample.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "api/Color.hpp"

void SimpleAnimationExample::AnimationScene::onRender() {
    Renderer::drawRect(Rect(0, animation.getValue(), 100, 100), Color::Red);
}

void SimpleAnimationExample::AnimationScene::onUpdate() {
    animation.update(y, 10.0F);

    if(animation.getValue() <= 1.0f) {
        y = 600.0F;
    } else if(animation.getValue() >= 599.0f) {
        y = 0.0F;
    }
}

void SimpleAnimationExample::run() {

    Window::Config config;
    config.title = "Simple Animation Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<SimpleAnimationExample::AnimationScene>();
    Window::show();
    Window::shutdown();
}