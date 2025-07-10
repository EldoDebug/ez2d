#include "FontExample.hpp"
#include "FontManager.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "api/Color.hpp"

void FontExample::run() {

    Window::Config config;

    config.title = "Font Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<FontExample::FontScene>();
    Window::show();
    Window::shutdown();
}

void FontExample::FontScene::onInit() {
    font = FontManager::load("test", "path/to/your/font.ttf");
}

void FontExample::FontScene::onRender() {
    if(font) {
        Renderer::drawText("Hello, World!", Point(100, 100), font, Color::White, 24);
    }
}