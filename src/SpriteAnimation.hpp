#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "Sprite.hpp"

class Rect;

class SpriteAnimation {
    public:
        struct AnimationFrame {
            float duration;
            int frameIndex;
        };
        
        struct Animation {
            std::vector<AnimationFrame> frames;
            bool loop;
            
            Animation() : loop(true) {}
        };
        SpriteAnimation(std::shared_ptr<Sprite> sprite);
        
        void addAnimation(const std::string& name, bool loop = true);
        void addFrame(const std::string& animationName, int index, float duration);
        void setCurrentAnimation(const std::string& name);
        
        void update();
        int getCurrentFrameIndex() const;
        void reset();
        void resetAnimation(const std::string& name);
        bool isFinished() const;
        void draw(Rect rect) const;
        
        std::string getCurrentAnimationName() const;
    
    private:
        std::map<std::string, Animation> animations;
        std::string currentAnimationName;
        float timer;
        size_t currentFrame;
        bool finished;
        std::shared_ptr<Sprite> sprite;
};