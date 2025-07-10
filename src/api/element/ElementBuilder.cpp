#include "ElementBuilder.hpp"
#include "../../TextureManager.hpp"
#include "../../Sprite.hpp"
#include "TextElement.hpp"
#include <memory>

std::shared_ptr<RootElement> ElementBuilder::createRoot(float screenWidth, float screenHeight) {
    return std::make_shared<RootElement>(screenWidth, screenHeight);
}

std::shared_ptr<Element> ElementBuilder::createPanel(float width, float height, Color color) {
    auto panel = std::make_shared<Element>();
    panel->setSize(width, height);
    panel->setColor(color);
    return panel;
}

std::shared_ptr<Element> ElementBuilder::createImage(std::shared_ptr<Texture> texture, float width, float height) {
    auto image = std::make_shared<Element>();
    image->setTexture(texture);
    
    if (width > 0 && height > 0) {
        image->setSize(width, height);
    } else if (texture) {
        image->setSize(texture->size.width, texture->size.height);
    }
    
    return image;
}

std::shared_ptr<Element> ElementBuilder::createSprite(std::shared_ptr<Sprite> sprite, int index, float width, float height) {
    auto element = std::make_shared<Element>();
    element->setSprite(sprite, index);

    if (width > 0 && height > 0) {
        element->setSize(width, height);
    } else if (sprite) {
        element->setSize(sprite->size.width, sprite->size.height);
    }
    
    return element;
}

std::shared_ptr<TextElement> ElementBuilder::createText(const std::string& text, std::shared_ptr<Font> font, float fontSize, const Color& color) {
    auto textElement = std::make_shared<TextElement>(text, font, fontSize, color);
    return textElement;
}

std::shared_ptr<VBoxContainer> ElementBuilder::createVBox(float spacing) {
    auto vbox = std::make_shared<VBoxContainer>();
    if (spacing > 0) {
        vbox->setSpacing(spacing);
    }
    return vbox;
}

std::shared_ptr<HBoxContainer> ElementBuilder::createHBox(float spacing) {
    auto hbox = std::make_shared<HBoxContainer>();
    if (spacing > 0) {
        hbox->setSpacing(spacing);
    }
    return hbox;
}

std::shared_ptr<CenterContainer> ElementBuilder::createCenter() {
    return std::make_shared<CenterContainer>();
}

std::shared_ptr<GridContainer> ElementBuilder::createGrid(int columns, int rows) {
    return std::make_shared<GridContainer>(columns, rows);
}

void ElementBuilder::setFlex(std::shared_ptr<Element> element, float grow, float shrink) {
    element->setFlexGrow(grow);
    element->setFlexShrink(shrink);
}

void ElementBuilder::setSize(std::shared_ptr<Element> element, float width, float height) {
    element->setSize(width, height);
}

void ElementBuilder::setPadding(std::shared_ptr<Element> element, float padding) {
    element->setPadding(padding);
}

void ElementBuilder::setMargin(std::shared_ptr<Element> element, float margin) {
    element->setMargin(margin);
}

void ElementBuilder::setAutoSize(std::shared_ptr<Element> element) {
    element->setAutoSize();
}

void ElementBuilder::setFillParent(std::shared_ptr<Element> element) {
    element->setFillParent();
}

void ElementBuilder::setFillWidth(std::shared_ptr<Element> element) {
    element->setFillWidth();
}

void ElementBuilder::setFillHeight(std::shared_ptr<Element> element) {
    element->setFillHeight();
}

std::shared_ptr<Element> ElementBuilder::createTopBar(float height, Color backgroundColor) {
    auto topBar = std::make_shared<HBoxContainer>();
    topBar->setHeight(height);
    topBar->setColor(backgroundColor);
    topBar->setAlignment(Element::AlignItems::Center);
    return topBar;
}

std::shared_ptr<Element> ElementBuilder::createBottomBar(float height, Color backgroundColor) {
    auto bottomBar = std::make_shared<HBoxContainer>();
    bottomBar->setHeight(height);
    bottomBar->setColor(backgroundColor);
    bottomBar->setAlignment(Element::AlignItems::Center);
    return bottomBar;
}

std::shared_ptr<Element> ElementBuilder::createSidebar(float width, bool isLeft, Color backgroundColor) {
    auto sidebar = std::make_shared<VBoxContainer>();
    sidebar->setWidth(width);
    sidebar->setColor(backgroundColor);
    return sidebar;
}

std::shared_ptr<Element> ElementBuilder::createModal(float width, float height, Color backgroundColor) {

    auto overlay = std::make_shared<CenterContainer>();
    overlay->setColor(Color(0, 0, 0, 128));
    
    auto modal = std::make_shared<Element>();
    modal->setSize(width, height);
    modal->setColor(backgroundColor);
    modal->setCornerRadius(8.0f);
    
    overlay->addChild(modal);
    return overlay;
}

template <typename T, typename... Args>
std::shared_ptr<T> ElementBuilder::createElement(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}