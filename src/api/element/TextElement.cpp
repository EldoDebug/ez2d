#include "TextElement.hpp"
#include "Renderer.hpp"
#include "api/Point.hpp"
#include "api/Rect.hpp"
#include "api/Size.hpp"
#include <string>

std::string TextElement::getText() const {
    return text;
}

void TextElement::setText(const std::string& newText) {
    text = newText;
    updateFontBounds();
}

std::shared_ptr<Font> TextElement::getFont() const {
    return font;
}

float TextElement::getFontSize() const {
    return fontSize;
}

void TextElement::setFontSize(float size) {
    fontSize = size;
    updateFontBounds();
}

Color TextElement::getFontColor() const {
    return fontColor;
}

void TextElement::setFontColor(const Color& color) {
    fontColor = color;
}

void TextElement::drawContent() {
    if(font) {
        Rect bounds = getAbsoluteBounds();
        Renderer::drawCenteredText(text, Point(bounds.x + bounds.width / 2, bounds.y), font, fontColor, fontSize);
    }
}

void TextElement::updateFontBounds() {
    if(font) {
        Size bounds = Renderer::getTextSize(text, font, fontSize);
        setSize(bounds.width, bounds.height);
    }
}