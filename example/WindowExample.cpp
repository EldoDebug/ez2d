#include "WindowExample.hpp"
#include "Window.hpp"

void WindowExample::run() {
    
    Window::Config config;

    config.rendererType = Window::RendererType::Auto;
    config.title = "Window Example";
    config.size = Size(1280, 720);
    config.resizable = true;
    config.fullscreen = false;
    config.fullscreenMode = Window::FullscreenMode::Borderless;
    config.vsync = true;

    Window::create(config);
    Window::init();

    Window::setTitle(Window::getTitle() + " - " + Window::getRendererType());

    Window::show();
    Window::shutdown();
}