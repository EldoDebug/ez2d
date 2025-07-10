#include "Keyboard.hpp"
#include <SDL3/SDL.h>

std::unordered_set<SDL_Keycode> Keyboard::pressedKeys;
std::unordered_set<SDL_Keycode> Keyboard::pressedThisFrame;
std::unordered_set<SDL_Keycode> Keyboard::releasedThisFrame;

bool Keyboard::isDown(SDL_Keycode key) {
    return pressedKeys.find(key) != pressedKeys.end();
}

bool Keyboard::isPressed(SDL_Keycode key) {
    auto it = pressedThisFrame.find(key);
    if (it != pressedThisFrame.end()) {
        pressedThisFrame.erase(it);
        return true;
    }
    return false;
}

bool Keyboard::isReleased(SDL_Keycode key) {
    auto it = releasedThisFrame.find(key);
    if (it != releasedThisFrame.end()) {
        releasedThisFrame.erase(it);
        return true;
    }
    return false;
}

void Keyboard::onDown(SDL_Keycode key) {
    if (pressedKeys.find(key) == pressedKeys.end()) {
        pressedThisFrame.insert(key);
    }
    pressedKeys.insert(key);
}

void Keyboard::onUp(SDL_Keycode key) {
    if (pressedKeys.find(key) != pressedKeys.end()) {
        releasedThisFrame.insert(key);
    }
    pressedKeys.erase(key);
    pressedThisFrame.erase(key);
}

void Keyboard::reset() {
    pressedThisFrame.clear();
    releasedThisFrame.clear();
}
