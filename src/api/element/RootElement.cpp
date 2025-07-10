#include "RootElement.hpp"

RootElement::RootElement(float screenWidth, float screenHeight) 
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    setSize(screenWidth, screenHeight);
}

void RootElement::resize(float width, float height) {
    screenWidth = width;
    screenHeight = height;
    setSize(width, height);
    updateLayout();
}

void RootElement::updateLayout() {
    calculateLayout(screenWidth, screenHeight);
}

void RootElement::draw() {
    Element::update();
    Element::draw();
}