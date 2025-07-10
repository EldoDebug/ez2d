#include "TextureExample.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "TextureManager.hpp"
#include "api/Rect.hpp"

void TextureExample::TextureScene::onInit() {
    texture = TextureManager::load("id", "path/to/your/texture.png");
}

void TextureExample::TextureScene::onRender() {
    if (texture) {
        Renderer::drawTexture(texture, Rect(100, 100, 200, 200));
    }
}

void TextureExample::TextureScene::onExit() {
    TextureManager::unload("id");
}

void TextureExample::run() {
    
    Window::Config config;
    config.title = "Texture Example";
    config.size = Size(800, 600);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<TextureScene>();
    Window::show();
    Window::shutdown();
}