#include "SpriteAnimation.hpp"
#include "Sprite.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "api/Rect.hpp"
#include <memory>

SpriteAnimation::SpriteAnimation(std::shared_ptr<Sprite> sprite)
    : timer(0.0f), currentFrame(0), finished(false), sprite(sprite) {}

void SpriteAnimation::addAnimation(const std::string& name, bool loop) {
    animations[name] = Animation();
    animations[name].loop = loop;
    
    if (currentAnimationName.empty()) {
        currentAnimationName = name;
    }
}

void SpriteAnimation::addFrame(const std::string& animationName, int index, float duration) {
    auto it = animations.find(animationName);
    if (it != animations.end()) {
        it->second.frames.push_back({duration, index});
    }
}

void SpriteAnimation::setCurrentAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end() && currentAnimationName != name) {
        currentAnimationName = name;
        reset();
    }
}

void SpriteAnimation::update() {
    if (animations.empty() || currentAnimationName.empty() || finished) return;
    
    auto& currentAnimation = animations[currentAnimationName];
    if (currentAnimation.frames.empty()) return;
    
    timer += Window::getDeltaTime() / 1000.0f;
    while (timer >= currentAnimation.frames[currentFrame].duration) {
        timer -= currentAnimation.frames[currentFrame].duration;
        currentFrame++;
        if (currentFrame >= currentAnimation.frames.size()) {
            if (currentAnimation.loop) {
                currentFrame = 0;
            } else {
                currentFrame = currentAnimation.frames.size() - 1;
                finished = true;
                break;
            }
        }
    }
}

int SpriteAnimation::getCurrentFrameIndex() const {
    if (animations.empty() || currentAnimationName.empty()) return 0;
    
    auto it = animations.find(currentAnimationName);
    if (it == animations.end() || it->second.frames.empty()) return 0;
    
    return it->second.frames[currentFrame].frameIndex;
}

void SpriteAnimation::reset() {
    timer = 0.0f;
    currentFrame = 0;
    finished = false;
}

void SpriteAnimation::resetAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        if (currentAnimationName == name) {
            reset();
        }
    }
}

bool SpriteAnimation::isFinished() const {
    return finished;
}

std::string SpriteAnimation::getCurrentAnimationName() const {
    return currentAnimationName;
}

void SpriteAnimation::draw(Rect rect) const {
    if (animations.empty() || currentAnimationName.empty()) return;
    
    auto it = animations.find(currentAnimationName);
    if (it == animations.end() || it->second.frames.empty()) return;
    
    Renderer::drawSprite(sprite, rect, getCurrentFrameIndex());
}
