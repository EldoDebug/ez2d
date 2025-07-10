#include "SpriteExample.hpp"
#include "Renderer.hpp"
#include "SpriteManager.hpp"
#include "Window.hpp"

void SpriteExample::run() {

    Window::Config config;

    config.title = "Sprite Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<SpriteExample::SpriteScene>();
    Window::show();
    Window::shutdown();
}

void SpriteExample::SpriteScene::onInit() {
    sprite = SpriteManager::load("test", "path/to/your/sprite.png", 64, 64);
}

void SpriteExample::SpriteScene::onRender() {
    if (sprite) {
        Renderer::drawSprite(sprite, Rect(0, 0, 64, 64), 0);
    }
}