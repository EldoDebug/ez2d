#include "InputExample.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "api/Color.hpp"
#include "api/Point.hpp"
#include "api/Rect.hpp"

void InputExample::run() {

    Window::Config config;

    config.title = "Input Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<InputExample::InputScene>();
    Window::show();
    Window::shutdown();
}

void InputExample::InputScene::onRender() {
    Renderer::drawRect(Rect(x, y, 100, 100), Color::Red);
    Renderer::drawCircle(Point(Mouse::getX(), Mouse::getY()), 50.0F, circleColor);
}

void InputExample::InputScene::onUpdate() {

    if(Keyboard::isDown(Keyboard::A)) {
        x -= Window::getDeltaTime();
    }

    if(Keyboard::isDown(Keyboard::D)) {
        x += Window::getDeltaTime();
    }

    if(Keyboard::isDown(Keyboard::W)) {
        y -= Window::getDeltaTime();
    }

    if(Keyboard::isDown(Keyboard::S)) {
        y += Window::getDeltaTime();
    }

    if(Mouse::isDown(Mouse::Left)) {
        circleColor = Color::Blue;
    } else if(Mouse::isDown(Mouse::Right)) {
        circleColor = Color::Green;
    } else {
        circleColor = Color::Red;
    }
}