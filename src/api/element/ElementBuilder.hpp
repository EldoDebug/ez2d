#pragma once

#include "Element.hpp"
#include "Container.hpp"
#include "RootElement.hpp"
#include "TextElement.hpp"
#include <memory>

class ElementBuilder {

    public:
        static std::shared_ptr<RootElement> createRoot(float screenWidth = 0, float screenHeight = 0);
        
        static std::shared_ptr<Element> createPanel(float width, float height, Color color = Color(255, 255, 255, 0));
        static std::shared_ptr<Element> createImage(std::shared_ptr<Texture> texture, float width = -1, float height = -1);
        static std::shared_ptr<Element> createSprite(std::shared_ptr<Sprite> sprite, int index, float width = -1, float height = -1);
        
        static std::shared_ptr<TextElement> createText(const std::string& text, std::shared_ptr<Font> font, float fontSize, const Color& color);

        static std::shared_ptr<VBoxContainer> createVBox(float spacing = 0);
        static std::shared_ptr<HBoxContainer> createHBox(float spacing = 0);
        static std::shared_ptr<CenterContainer> createCenter();
        static std::shared_ptr<GridContainer> createGrid(int columns, int rows = -1);
        
        static void setFlex(std::shared_ptr<Element> element, float grow = 1.0f, float shrink = 1.0f);
        static void setSize(std::shared_ptr<Element> element, float width, float height);
        static void setPadding(std::shared_ptr<Element> element, float padding);
        static void setMargin(std::shared_ptr<Element> element, float margin);
        
        static void setAutoSize(std::shared_ptr<Element> element);
        static void setFillParent(std::shared_ptr<Element> element);
        static void setFillWidth(std::shared_ptr<Element> element);
        static void setFillHeight(std::shared_ptr<Element> element);
        
        static std::shared_ptr<Element> createTopBar(float height, Color backgroundColor = Color(50, 50, 50, 200));
        static std::shared_ptr<Element> createBottomBar(float height, Color backgroundColor = Color(50, 50, 50, 200));
        static std::shared_ptr<Element> createSidebar(float width, bool isLeft = true, Color backgroundColor = Color(40, 40, 40, 200));
        
        static std::shared_ptr<Element> createModal(float width, float height, Color backgroundColor = Color(0, 0, 0, 128));

        template <typename T, typename... Args>
        static std::shared_ptr<T> createElement(Args&&... args);
    };