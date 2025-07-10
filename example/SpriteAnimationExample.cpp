#include "SpriteAnimationExample.hpp"
#include "SpriteAnimation.hpp"
#include "SpriteManager.hpp"
#include "Window.hpp"
#include "api/Rect.hpp"
#include <memory>

void SpriteAnimationExample::run() {

    Window::Config config;

    config.title = "Sprite Animation Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<SpriteAnimationExample::SpriteAnimationScene>();
    Window::show();
    Window::shutdown();
}

void SpriteAnimationExample::SpriteAnimationScene::onInit() {
    
    sprite = SpriteManager::load("test", "path/to/your/sprite.png", 64, 64);
    
    if (sprite) {
        
        spriteAnimation = std::make_shared<SpriteAnimation>(sprite);

        spriteAnimation->addAnimation("walk", true);
        spriteAnimation->addFrame("walk", 0, 0.1F);
        spriteAnimation->addFrame("walk", 1, 0.1F);
        spriteAnimation->addFrame("walk", 2, 0.1F);
        spriteAnimation->addFrame("walk", 3, 0.1F);

        spriteAnimation->addAnimation("idle", true);
        spriteAnimation->addFrame("idle", 4, 0.5F);
        spriteAnimation->addFrame("idle", 5, 0.5F);

        spriteAnimation->setCurrentAnimation("idle");
    }
}

void SpriteAnimationExample::SpriteAnimationScene::onRender() {
    if (spriteAnimation) {
        spriteAnimation->draw(Rect(0, 0, 64, 64));
    }
}