#pragma once
#include "Element.hpp"
#include "Font.hpp"
#include "api/Color.hpp"
#include <memory>
#include <string>

class TextElement : public Element  {

    private:
        std::string text;
        std::shared_ptr<Font> font;
        float fontSize;
        Color fontColor;

    public:
        TextElement(const std::string& text, std::shared_ptr<Font> font, float fontSize, const Color& color)
            : Element(), text(text), font(font), fontSize(fontSize), fontColor(color) {
                updateFontBounds();
            }

        std::string getText() const;
        void setText(const std::string& newText);
        std::shared_ptr<Font> getFont() const;
        float getFontSize() const;
        void setFontSize(float size);
        Color getFontColor() const;
        void setFontColor(const Color& color);

        void drawContent() override;

    private:
        void updateFontBounds();
};