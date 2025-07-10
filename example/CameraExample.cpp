#include "CameraExample.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "api/Color.hpp"
#include "Keyboard.hpp"

void CameraExample::run() {

    Window::Config config;

    config.rendererType = Window::RendererType::OpenGL;
    config.title = "Camera Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<CameraExample::CameraScene>();
    Window::show();
    Window::shutdown();
}

void CameraExample::CameraScene::onInit() {
}

void CameraExample::CameraScene::onRender() {
    
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 40; ++x) {
            if (MAP[y][x] == 1) {
                Renderer::drawRect(Rect(x * 32, y * 32, 32, 32), Color(124, 74, 43));
            } else if (MAP[y][x] == 2) {
                Renderer::drawRect(Rect(x * 32, y * 32, 32, 32), Color::Green);
            } else if (MAP[y][x] == 3) {
                Renderer::drawRect(Rect(x * 32, y * 32, 32, 32), Color::Blue);
            } else if (MAP[y][x] == 4) {
                Renderer::drawRect(Rect(x * 32, y * 32, 32, 32), Color::Red);
            }
        }
    }
}

void CameraExample::CameraScene::onUpdate() {
    
    Camera& cam = getCamera();
    
    if (Keyboard::isDown(Keyboard::A)) {
        cam.point.x -= 1.0f * Window::getDeltaTime();
    }

    if (Keyboard::isDown(Keyboard::D)) {
        cam.point.x += 1.0f * Window::getDeltaTime();
    }

    if (Keyboard::isDown(Keyboard::W)) {
        cam.point.y -= 1.0f * Window::getDeltaTime();
    }

    if (Keyboard::isDown(Keyboard::S)) {
        cam.point.y += 1.0f * Window::getDeltaTime();
    }

    if(Keyboard::isDown(Keyboard::Q)) {
        cam.zoom -= 0.01f * Window::getDeltaTime();
    }

    if(Keyboard::isDown(Keyboard::E)) {
        cam.zoom += 0.01f * Window::getDeltaTime();
    }

    if(Keyboard::isDown(Keyboard::Left)) {
        cam.angle -= 1.0f * Window::getDeltaTime();
    }

    if(Keyboard::isDown(Keyboard::Right)) {
        cam.angle += 1.0f * Window::getDeltaTime();
    }
}