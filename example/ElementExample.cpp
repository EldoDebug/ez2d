#include "ElementExample.hpp"
#include "FontManager.hpp"
#include "Window.hpp"
#include "api/Color.hpp"
#include "api/element/Element.hpp"
#include "api/element/ElementBuilder.hpp"

void ElementExample::run() {
    Window::Config config;
    config.title = "Element Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<ElementExample::ElementScene>();
    Window::show();
    Window::shutdown();
}

void ElementExample::ElementScene::onInit() {

    FontManager::load("test", "assets/Roboto-Regular.ttf");

    rootElement = ElementBuilder::createRoot(Window::getWidth(), Window::getHeight());

    rootElement->setColor(Color::Green);

    auto centerContainer = ElementBuilder::createCenter();

    centerContainer->setColor(Color::Red);
    centerContainer->setMargin(20.0F);
    centerContainer->setFillParent();
    centerContainer->setAlignItems(Element::AlignItems::Center);
    centerContainer->setJustifyContent(Element::JustifyContent::Center);

    auto centerInCenter = ElementBuilder::createCenter();

    centerInCenter->setColor(Color::Yellow);
    centerInCenter->setFillParent();
    centerInCenter->setMargin(20.0F);

    rootElement->addChild(centerContainer);

    auto textElement = ElementBuilder::createText("Hello World", FontManager::get("test"), 32.0F, Color::White);

    textElement->setColor(Color::Blue);
    textElement->setMargin(10.0F);

    centerContainer->addChild(centerInCenter);
    centerInCenter->addChild(textElement);
    rootElement->updateLayout();
}

void ElementExample::ElementScene::onUpdate() {

}

void ElementExample::ElementScene::onRender() {
    if(rootElement) {
        rootElement->draw();
    }
}

void ElementExample::ElementScene::onResize(int width, int height) {
    if(rootElement) {
        rootElement->resize(width, height);
    }
}