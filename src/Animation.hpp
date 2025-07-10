#pragma once

enum class Easing {
    None,
    EaseLinear,
    EaseInSine,
    EaseOutSine,
    EaseInOutSine,
    EaseInQuad,
    EaseOutQuad,
    EaseInOutQuad,
    EaseInCubic,
    EaseOutCubic,
    EaseInOutCubic,
    EaseInQuart,
    EaseOutQuart,
    EaseInOutQuart,
    EaseInQuint,
    EaseOutQuint,
    EaseInOutQuint,
    EaseInExpo,
    EaseOutExpo,
    EaseInOutExpo,
    EaseInCirc,
    EaseOutCirc,
    EaseInOutCirc,
    EaseInBack,
    EaseOutBack,
    EaseInOutBack,
    EaseInElastic,
    EaseOutElastic,
    EaseInOutElastic,
};

class Animation {

    public:
        float duration;
        float start;
        float change;
        float timePassed = 0.0F;
        Easing easing;
        float value = 0.0F;

    public:
        Animation() = default;
        
        Animation(float duration, float start, float end, Easing easing) {
            this->duration = duration;
            this->start = start;
            this->change = end - start;
            this->easing = easing;
            this->value = start;
        }

        void update();
        
        float getValue() const;
        
        bool isFinished() const;
        
        float getEnd() const;
        
        void reset(float newStart, float newEnd);
        void reset(float newStart, float newEnd, Easing newEasing);
        
        virtual float animate(float x);
};