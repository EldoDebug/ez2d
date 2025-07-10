#include "ResourceLoaderExample.hpp"
#include "FontManager.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "TextureManager.hpp"
#include "api/Point.hpp"

void ResourceLoaderExample::run() {
    Window::Config config;
    config.title = "Resource Loader Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<ResourceLoaderExample::ResourceLoaderScene>();
    Window::show();
    Window::shutdown();
}

void ResourceLoaderExample::ResourceLoaderScene::onInit() {
    loader.addTexture("egg", "assets/egg.png");
    loader.addTexture("banana", "assets/banana.png");
    loader.addFont("Roboto", "assets/Roboto-Regular.ttf");
    loader.start();
}

void ResourceLoaderExample::ResourceLoaderScene::onRender() {
    
    if(loader.isFinished()){
        texture = TextureManager::get("egg");
        texture2 = TextureManager::get("banana");
        font = FontManager::get("Roboto");
    }

    if (texture) {
        Renderer::drawTexture(texture, Rect(100, 100, 200, 200));
    }
    if (texture2) {
        Renderer::drawTexture(texture2, Rect(400, 100, 200, 200));
    }
    if (font) {
        Renderer::drawText("Hello, Resource Loader!", Point(100, 350), font, Color::White, 32);
    }
}