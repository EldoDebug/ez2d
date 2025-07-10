#include "AnimationExample.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "api/Color.hpp"
#include "api/Rect.hpp"

void AnimationExample::run() {

    Window::Config config;

    config.title = "Animation Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<AnimationExample::AnimationScene>();
    Window::show();
    Window::shutdown();
}

void AnimationExample::AnimationScene::onInit() {
    animation = Animation(1000.0f, 0.0f, 600.0f, Easing::EaseInOutQuad);
}

void AnimationExample::AnimationScene::onRender() {
    Renderer::drawRect(Rect(0, animation.getValue(), 100, 100), Color::Red);
}

void AnimationExample::AnimationScene::onUpdate() {
    
    animation.update();

    if(animation.isFinished() && animation.getEnd() == 600.0f) {
        animation.reset(600.0f, 0.0f);
    } else if (animation.isFinished() && animation.getEnd() == 0.0f) {
        animation.reset(0.0f, 600.0f);
    }
}