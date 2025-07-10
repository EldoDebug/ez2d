#pragma once

#include "Camera.hpp"
#include "api/Point.hpp"
#include <string>

class Window;

class Scene {

    private:

    public:
        Scene() = default;
        virtual ~Scene() = default;

        Camera& getCamera() { return camera; }
        const Camera& getCamera() const { return camera; }

        virtual void onInit() {}
        virtual void onUpdate() {}
        virtual void onRender() {}
        virtual void onMousePressed(Point point, int button) {}
        virtual void onMouseReleased(Point point, int button) {}
        virtual void onMouseMoved(Point point) {}
        virtual void onKeyPressed(int keyCode, int scanCode, int modifiers, bool repeat) {}
        virtual void onKeyReleased(int keyCode, int scanCode, int modifiers) {}
        virtual void onTextInput(const std::string& text) {}
        virtual void onFocusGained() {}
        virtual void onFocusLost() {}
        virtual void onResize(int width, int height) {}
        virtual void onExit() {}

        virtual bool shouldPause() const { return false; }

    protected:
        Camera camera;
};