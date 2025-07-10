#include "Mouse.hpp"
#include "api/Point.hpp"
#include <unordered_set>

std::unordered_set<int> Mouse::pressedButtons;
std::unordered_set<int> Mouse::pressedThisFrame;
std::unordered_set<int> Mouse::releasedThisFrame;
Point Mouse::mousePoint = { 0.0F, 0.0F };
Point Mouse::scrollPoint = { 0.0F, 0.0F };

bool Mouse::isDown(int button) {
    return pressedButtons.find(button) != pressedButtons.end();
}

bool Mouse::isPressed(int button) {
    if (pressedThisFrame.find(button) != pressedThisFrame.end()) {
        pressedThisFrame.erase(button);
        return true;
    }
    return false;
}

bool Mouse::isReleased(int button) {
    if (releasedThisFrame.find(button) != releasedThisFrame.end()) {
        releasedThisFrame.erase(button);
        return true;
    }
    return false;
}

void Mouse::onDown(int button) {
    if (pressedButtons.find(button) == pressedButtons.end()) {
        pressedThisFrame.insert(button);
    }
    pressedButtons.insert(button);
}

void Mouse::onUp(int button) {
    pressedButtons.erase(button);
    releasedThisFrame.insert(button);
}

void Mouse::reset() {
    pressedThisFrame.clear();
    releasedThisFrame.clear();
    scrollPoint = { 0.0F, 0.0F };
}

float Mouse::getX() {
    return mousePoint.x;
}

float Mouse::getY() {
    return mousePoint.y;
}

void Mouse::setPosition(Point point) {
    mousePoint = point;
}

float Mouse::getScrollX() {
    float val = scrollPoint.x;
    scrollPoint.x = 0;
    return val;
}

float Mouse::getScrollY() {
    float val = -scrollPoint.y;
    scrollPoint.y = 0;
    return val;
}

bool Mouse::isInside(Rect rect) {
    return mousePoint.x >= rect.x && mousePoint.x <= rect.x + rect.width &&
           mousePoint.y >= rect.y && mousePoint.y <= rect.y + rect.height;
}