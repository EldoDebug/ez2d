#include "SimpleAnimation.hpp"
#include "Window.hpp"

void SimpleAnimation::update(float value, float speed) {
    if (firstTick) {
        currentValue = value;
        firstTick = false;
    }
    else {
        currentValue += (((value - currentValue) * (speed / 1000.0F)) * Window::getDeltaTime());
    }
}

float SimpleAnimation::getValue() {
    return currentValue;
}

void SimpleAnimation::reset() {
    firstTick = true;
}