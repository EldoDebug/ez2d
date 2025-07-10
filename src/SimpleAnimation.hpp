#pragma once

class SimpleAnimation {
    private:
        float currentValue;
        bool firstTick = true;

    public:
        void update(float value, float duration);
        float getValue();
        void reset();
};