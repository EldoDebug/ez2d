#pragma once

#include "SDL3/SDL_video.h"
#include "Scene.hpp"
#include "api/Point.hpp"
#include "api/Size.hpp"
#include "bgfx/bgfx.h"
#include <memory>
#include <string>
#include <vector>
#include <type_traits>
#include <utility>

class Window {

    public:
        enum class RendererType {
            Auto,
            OpenGL,
            Vulkan,
            DirectX_11,
            DirectX_12,
            Metal
        };

        enum class FullscreenMode {
            Windowed,
            Fullscreen,
            Borderless
        };

        struct Config {
            std::string title = "ez2d";
            Size size = Size(1280, 720);
            bool resizable = true;
            bool fullscreen = false;
            FullscreenMode fullscreenMode = FullscreenMode::Borderless;
            bool vsync = true;
            int targetFps = 60;
            RendererType rendererType = RendererType::Auto;
            Size virtualSize = size;
            bool fixedCoordinateMode = false;
        };
        
        static void create(const Config& config);
        static void init();
        static void show();
        static void shutdown();

        static std::string getTitle();
        static void setTitle(const std::string& title); 
        static Size getSize();       
        static int getWidth();
        static int getHeight();
        
        static void setVsync(bool enable);
        static bool isVsync();

        static void setTargetFps(int fps);
        static int getTargetFps();

        static bool isFullscreen();
        static void setFullscreen(bool enable);
        
        static std::string getRendererType();

        static float getDeltaTime();
        static int getFPS();

        static void popScene();
        static std::unique_ptr<Scene> getTopScene();

        template<typename T>
        static bool isTopScene() {
            static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");
            if (sceneStack.empty()) return false;
            return dynamic_cast<T*>(sceneStack.back().get()) != nullptr;
        }

        template<typename T, typename... Args>
        static void setScene(Args&&... args) {
            static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");
            if (!isTopScene<T>()) {
                _setScene(std::make_unique<T>(std::forward<Args>(args)...));
            }
        }

        template<typename T, typename... Args>
        static void pushScene(Args&&... args) {
            if (!isTopScene<T>())  {
                static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");
                _pushScene(std::make_unique<T>(std::forward<Args>(args)...));
            }
        }
        
        template<typename T, typename... Args>
        static bool isScene(Args&&... args) {
            static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");
            if (sceneStack.empty()) return false;
            return dynamic_cast<T*>(sceneStack.back().get()) != nullptr;
        }   
        
    private:
        Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        static SDL_Window* window;
        static bool closed;
        static Config config;        
        static Uint64 lastSDLTime;
        static float deltaTime;
        static float fps;
        static Uint64 lastFpsUpdateTime;
        static int frameCount;
        static float accumulatedFps;

        static std::vector<std::unique_ptr<Scene>> sceneStack;
        static Size actualWindowSize;

        static void _pushScene(std::unique_ptr<Scene> newScene);
        static void _setScene(std::unique_ptr<Scene> newScene);
        static void handleUpdate();
        static void handleRender();
        static void handleMousePressed(Point point, int button);
        static void handleMouseReleased(Point point, int button);
        static void handleMouseMoved(Point point);
        static void handleKeyPressed(int keyCode, int scanCode, int modifiers, bool repeat);
        static void handleKeyReleased(int keyCode, int scanCode, int modifiers);
        static void handleTextInput(const std::string& text);
        static void handleFocusGained();
        static void handleFocusLost();
        static void handleResize(int width, int height);

        static bgfx::RendererType::Enum convertRendererType(RendererType type);
        static void updateDeltaTime();
};