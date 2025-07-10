#include "Animation.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>
#include "Window.hpp"

void Animation::update() {

    timePassed += Window::getDeltaTime();

    float t = std::min(timePassed / duration, 1.0f);
    value = animate(t) * change + start;
}

float Animation::getValue() const {
    return value;
}

bool Animation::isFinished() const {
    return timePassed >= duration;
}

float Animation::getEnd() const {
    return start + change;
}

void Animation::reset(float newStart, float newEnd) {
    this->start = newStart;
    this->change = newEnd - newStart;
    this->timePassed = 0.0f;
    this->value = newStart;
}

void Animation::reset(float newStart, float newEnd, Easing newEasing) {
    this->start = newStart;
    this->change = newEnd - newStart;
    this->easing = newEasing;
    this->timePassed = 0.0f;
    this->value = newStart;
}

float Animation::animate(float x) {
    switch (easing) {
        case Easing::None:
            return x;
            break;
        case Easing::EaseLinear:
            return x;
            break;
        case Easing::EaseInSine:
            return 1 - cos((x * std::numbers::pi) / 2);
            break;
        case Easing::EaseOutSine:
            return sin((x * std::numbers::pi) / 2);
            break;
        case Easing::EaseInOutSine:
            return -(cos(std::numbers::pi * x) - 1) / 2;
            break;
        case Easing::EaseInQuad:
            return x * x;
            break;
        case Easing::EaseOutQuad:
            return 1 - (1 - x) * (1 - x);
            break;
        case Easing::EaseInOutQuad:
            return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
            break;
        case Easing::EaseInCubic:
            return x * x * x;
            break;
        case Easing::EaseOutCubic:
            return 1 - pow(1 - x, 3);
            break;
        case Easing::EaseInOutCubic:
            return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
            break;
        case Easing::EaseInQuart:
            return x * x * x * x;
            break;
        case Easing::EaseOutQuart:
            return 1 - pow(1 - x, 4);
            break;
        case Easing::EaseInOutQuart:
            return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
            break;
        case Easing::EaseInQuint:
            return x * x * x * x * x;
            break;
        case Easing::EaseOutQuint:
            return 1 - pow(1 - x, 5);
            break;
        case Easing::EaseInOutQuint:
            return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
            break;
        case Easing::EaseInExpo:
            return x == 0 ? 0 : pow(2, 10 * x - 10);
            break;
        case Easing::EaseOutExpo:
            return x == 1 ? 1 : 1 - pow(2, -10 * x);
            break;
        case Easing::EaseInOutExpo:
            return x == 0
                    ? 0
                    : x == 1
                    ? 1
                    : x < 0.5 ? pow(2, 20 * x - 10) / 2
                    : (2 - pow(2, -20 * x + 10)) / 2;
            break;
        case Easing::EaseInCirc:
            return 1 - sqrt(1 - pow(x, 2));
            break;
        case Easing::EaseOutCirc:
            return sqrt(1 - pow(x - 1, 2));
            break;
        case Easing::EaseInOutCirc:
            return x < 0.5
                ? (1 - sqrt(1 - pow(2 * x, 2))) / 2
                : (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
            break;
        case Easing::EaseInBack: {
                float c1 = 1.70158F;
                float c3 = c1 + 1;
                return c3 * x * x * x - c1 * x * x;
            }
            break;
        case Easing::EaseOutBack: {
                float c1 = 1.70158F;
                float c3 = c1 + 1;
                return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
            }
            break;
        case Easing::EaseInOutBack: {
                float c1 = 1.70158F;
                float c2 = c1 * 1.525F;

                return x < 0.5F ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2 
                        : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
            };
            break;
        case Easing::EaseInElastic: {
                float c4 = (2 * std::numbers::pi) / 3;

                return x == 0
                    ? 0
                    : x == 1
                    ? 1
                    : -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
            }
            break;
        case Easing::EaseOutElastic: {
                float c4 = (2 * std::numbers::pi) / 3;
                return x == 0
                    ? 0
                    : x == 1
                    ? 1
                    : pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
            };
            break;
        case Easing::EaseInOutElastic: {
            float c5 = (2 * std::numbers::pi) / 4.5;
            return x == 0
                ? 0
                : x == 1
                ? 1
                : x < 0.5
                ? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
                : (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
            };
            break;
    }
    return 0.0F;
}