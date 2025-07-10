#include "Window.hpp"
#include "Renderer.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_hints.h"
#include "SDL3/SDL_video.h"
#include "Scene.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "api/Point.hpp"
#include "api/Rect.hpp"
#include "api/Size.hpp"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <utility>
#include "Renderer.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "Logger.hpp"
#include "Platform.hpp"

#if defined(WINDOWS)
#include <windows.h>
#endif

SDL_Window* Window::window = nullptr;
bool Window::closed = false;
Window::Config Window::config;
Uint64 Window::lastSDLTime = 0;
float Window::deltaTime = 0.0f;
float Window::fps = 0.0f;
Uint64 Window::lastFpsUpdateTime = 0;
int Window::frameCount = 0;
float Window::accumulatedFps = 0.0f;
std::vector<std::unique_ptr<Scene>> Window::sceneStack;
Size Window::actualWindowSize;

void Window::create(const Config& cfg) {
    config = cfg;
    config.virtualSize = config.size;
}

void Window::init() {

    Logger::init();

    SDL_Init(0);

    int sdlFlags = SDL_WINDOW_HIDDEN;
    int bgfxFlags = BGFX_RESET_MSAA_X16;

    if(config.fullscreen) {
        if (config.fullscreenMode == FullscreenMode::Fullscreen) {
            sdlFlags |= SDL_WINDOW_FULLSCREEN;
        } else if (config.fullscreenMode == FullscreenMode::Borderless) {
            sdlFlags |= SDL_WINDOW_BORDERLESS;
        }
    }

    if (config.resizable) {
        sdlFlags |= SDL_WINDOW_RESIZABLE;
    }

    if(config.vsync) {
        bgfxFlags |= BGFX_RESET_VSYNC;
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, config.vsync ? "1" : "0");

    window = SDL_CreateWindow(config.title.c_str(), (int) config.size.width, (int) config.size.height, sdlFlags);

    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    bgfx::PlatformData pd;

    #if defined(WINDOWS)
        pd.nwh = (HWND) SDL_GetPointerProperty( SDL_GetWindowProperties( window ), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL );
        pd.ndt = NULL;
    #elif defined(MACOS)
        pd.nwh = (__bridge NSWindow *) SDL_GetPointerProperty( SDL_GetWindowProperties( window ), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL );
        pd.ndt = NULL;
    #elif defined(LINUX)
        #if defined(LINUX_X11)
            pd.ndt = (Display *) SDL_GetPointerProperty( SDL_GetWindowProperties( window ), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL );
            pd.nwh = (Window) SDL_GetNumberProperty( SDL_GetWindowProperties( window  ), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0 );
        #elif defined(LINUX_WAYLAND)
            pd.ndt = (struct wl_display *) SDL_GetPointerProperty( SDL_GetWindowProperties( window ), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL );
            pd.nwh = (struct wl_surface *) SDL_GetPointerProperty( SDL_GetWindowProperties( window ), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL );
            pd.type = bgfx::NativeWindowHandleType::Enum::Wayland;            
        #endif
    #endif

    pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd);

    bgfx::Init init;
	init.type = convertRendererType(config.rendererType);
	init.vendorId = BGFX_PCI_ID_NONE;
	init.platformData.nwh = pd.nwh;
	init.platformData.ndt = pd.ndt;
	init.resolution.width = (int) config.size.width;
	init.resolution.height = (int) config.size.height;
    init.resolution.reset = bgfxFlags;

	if( !bgfx::init(init)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize bgfx");
        SDL_Quit();
        return;
    }

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000, 1.0f, 0);
    Renderer::init();

    actualWindowSize = config.size;

    lastSDLTime = SDL_GetTicksNS();
    lastFpsUpdateTime = lastSDLTime;
}

void Window::show() {

    SDL_ShowWindow(window);    
    SDL_Event event;

    while(!closed) {        
        Uint64 frameStart = SDL_GetTicksNS();        
        updateDeltaTime();
        Keyboard::reset();
        Mouse::reset();

        while(SDL_PollEvent(&event)) {
			switch(event.type) {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    closed = true;
                    break;
                case SDL_EVENT_WINDOW_RESIZED:
                    if(event.window.windowID == SDL_GetWindowID(window)) {
                        int width, height;
                        SDL_GetWindowSize(window, &width, &height);

                        actualWindowSize.width = static_cast<float>(width);
                        actualWindowSize.height = static_cast<float>(height);
                        if (!config.fixedCoordinateMode) {
                            config.size = actualWindowSize;
                        }
                        uint32_t flags = BGFX_RESET_MSAA_X16;
                        
                        if (config.vsync) {
                            flags |= BGFX_RESET_VSYNC;
                        }

                        bgfx::reset(uint32_t(width), uint32_t(height), flags);
                        handleResize(width, height);
                    }
                    break;                
                case SDL_EVENT_MOUSE_MOTION:
                    if(event.motion.windowID == SDL_GetWindowID(window)) {
                        float mx = static_cast<float>(event.motion.x);
                        float my = static_cast<float>(event.motion.y);
                        if (config.fixedCoordinateMode) {
                            mx = mx * (config.virtualSize.width / actualWindowSize.width);
                            my = my * (config.virtualSize.height / actualWindowSize.height);
                        }
                        Point point(mx, my);
                        Mouse::setPosition(point);
                        handleMouseMoved(point);
                    }
                    break;                
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if(event.button.windowID == SDL_GetWindowID(window)) {
                        int button = event.button.button;
                        float bx = static_cast<float>(event.button.x);
                        float by = static_cast<float>(event.button.y);
                        if (config.fixedCoordinateMode) {
                            bx = bx * (config.virtualSize.width / actualWindowSize.width);
                            by = by * (config.virtualSize.height / actualWindowSize.height);
                        }
                        Point point(bx, by);
                        Mouse::setPosition(point);
                        Mouse::onDown(button);
                        handleMousePressed(point, button);
                    }
                    break;               
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if(event.button.windowID == SDL_GetWindowID(window)) {
                        int button = event.button.button;
                        float ux = static_cast<float>(event.button.x);
                        float uy = static_cast<float>(event.button.y);
                        if (config.fixedCoordinateMode) {
                            ux = ux * (config.virtualSize.width / actualWindowSize.width);
                            uy = uy * (config.virtualSize.height / actualWindowSize.height);
                        }
                        Point point(ux, uy);
                        Mouse::setPosition(point);
                        Mouse::onUp(button);
                        handleMouseReleased(point, button);
                    }
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    if(event.wheel.windowID == SDL_GetWindowID(window)) {
                       float sx = 1.0f, sy = 1.0f;
                       if (config.fixedCoordinateMode) {
                           sx = config.virtualSize.width / actualWindowSize.width;
                           sy = config.virtualSize.height / actualWindowSize.height;
                       }
                       Mouse::_updateScroll(Point(event.wheel.x * sx, event.wheel.y * sy));
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if(event.key.windowID == SDL_GetWindowID(window)) {
                        SDL_Keycode keyCode = event.key.key;
                        SDL_Scancode scanCode = event.key.scancode;
                        SDL_Keymod modifiers = event.key.mod;
                        bool repeat = event.key.repeat;
                        Keyboard::onDown(keyCode);
                        handleKeyPressed(keyCode, scanCode, modifiers, repeat);
                    }
                    break;
                case SDL_EVENT_KEY_UP:
                    if(event.key.windowID == SDL_GetWindowID(window)) {
                        SDL_Keycode keyCode = event.key.key;
                        SDL_Scancode scanCode = event.key.scancode;
                        SDL_Keymod modifiers = event.key.mod;
                        Keyboard::onUp(keyCode);
                        handleKeyReleased(keyCode, scanCode, modifiers);
                    }
                    break;
                case SDL_EVENT_TEXT_INPUT:
                    if(event.text.windowID == SDL_GetWindowID(window)) {
                        std::string text(event.text.text);
                        handleTextInput(text);
                    }
                    break;
                case SDL_EVENT_WINDOW_FOCUS_GAINED:
                    if(event.window.windowID == SDL_GetWindowID(window)) {
                        handleFocusGained();
                    }
                    break;
                case SDL_EVENT_WINDOW_FOCUS_LOST:
                    if(event.window.windowID == SDL_GetWindowID(window)) {
                        handleFocusLost();
                    }
                    break;
                }
		}

        handleUpdate();

        uint16_t viewWidth = config.fixedCoordinateMode ? uint16_t(config.virtualSize.width) : uint16_t(config.size.width);
        uint16_t viewHeight = config.fixedCoordinateMode ? uint16_t(config.virtualSize.height) : uint16_t(config.size.height);
        bgfx::setViewRect(0, 0, 0, viewWidth, viewHeight);
        bgfx::touch(0);

        handleRender();

        bgfx::frame();        
        
        if (config.targetFps > 0 && !config.vsync) {
            double targetFrameTime = 1.0 / config.targetFps;
            double frameTime;
            
            do {
                Uint64 frameEnd = SDL_GetTicksNS();
                frameTime = (frameEnd - frameStart) / 1000000000.0;
            } while (frameTime < targetFrameTime);
        }
    }
}

void Window::shutdown() {
    
    while (!sceneStack.empty()) {
        sceneStack.back()->onExit();
        sceneStack.pop_back();
    }
    
    TextureManager::unloadAll();
    Renderer::shutdown();
	bgfx::shutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();
    Logger::shutdown();
}

std::string Window::getTitle() {
    return config.title;
}

void Window::setTitle(const std::string& title) {
    config.title = title;
    if (window) {
        SDL_SetWindowTitle(window, title.c_str());
    }
}

Size Window::getSize() {
    if (config.fixedCoordinateMode) {
        return config.virtualSize;
    } else {
        return actualWindowSize;
    }
}

int Window::getWidth() {
    return (int) config.size.width;
}

int Window::getHeight() {
    return (int) config.size.height;
}

bgfx::RendererType::Enum Window::convertRendererType(RendererType type) {
    switch (type) {
        case RendererType::OpenGL:
            return bgfx::RendererType::OpenGL;
        case RendererType::Vulkan:
            return bgfx::RendererType::Vulkan;
        case RendererType::DirectX_11:
            return bgfx::RendererType::Direct3D11;
        case RendererType::DirectX_12:
            return bgfx::RendererType::Direct3D12;
        case RendererType::Metal:
            return bgfx::RendererType::Metal;
        default:
            return bgfx::RendererType::Count;
    }
}

std::string Window::getRendererType() {
    switch(bgfx::getRendererType()) {
        case bgfx::RendererType::OpenGL:
            return "OpenGL";
        case bgfx::RendererType::Vulkan:
            return "Vulkan";
        case bgfx::RendererType::Direct3D11:
            return "DirectX 11";
        case bgfx::RendererType::Direct3D12:
            return "DirectX 12";
        case bgfx::RendererType::Metal:
            return "Metal";
        default:
            return "Unknown";
    }
}

void Window::updateDeltaTime() {

    Uint64 currentTime = SDL_GetTicksNS();
    deltaTime = (currentTime - lastSDLTime) / 1000000000.0f;
    
    frameCount++;
    if (deltaTime > 0.0f) {
        accumulatedFps += 1.0f / deltaTime;
    }
    
    if (currentTime - lastFpsUpdateTime >= 1000000000ULL) {
        if (frameCount > 0) {
            fps = accumulatedFps / frameCount;
        }
        frameCount = 0;
        accumulatedFps = 0.0f;
        lastFpsUpdateTime = currentTime;
    }
    
    lastSDLTime = currentTime;
}

void Window::setVsync(bool enable) {
    config.vsync = enable;

    int bgfxFlags = BGFX_RESET_MSAA_X16;
    
    if (config.vsync) {
        bgfxFlags |= BGFX_RESET_VSYNC;
    }

    bgfx::reset((int) config.size.width, (int) config.size.height, bgfxFlags);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, config.vsync ? "1" : "0");
}

bool Window::isVsync() {
    return config.vsync;
}

void Window::setTargetFps(int fps) {
    config.targetFps = fps;
}

int Window::getTargetFps() {
    return config.targetFps;
}

bool Window::isFullscreen() {
    return config.fullscreen;
}

void Window::setFullscreen(bool enable) {

    config.fullscreen = enable;

    int flags = SDL_WINDOW_HIDDEN;

    if (enable) {
        if (config.fullscreenMode == FullscreenMode::Fullscreen) {
            flags |= SDL_WINDOW_FULLSCREEN;
        } else if (config.fullscreenMode == FullscreenMode::Borderless) {
            flags |= SDL_WINDOW_BORDERLESS;
        }
    }

    SDL_SetWindowFullscreen(window, flags);
}

float Window::getDeltaTime() {
    return deltaTime * 1000.0f;
}

int Window::getFPS() {
    return static_cast<int>(fps);
}

void Window::_pushScene(std::unique_ptr<Scene> newScene) {
    if(newScene) {
        sceneStack.push_back(std::move(newScene));
        sceneStack.back()->onInit();
    }
}

void Window::popScene() {
    if (!sceneStack.empty()) {
        sceneStack.back()->onExit();
        sceneStack.pop_back();
    }
}

void Window::_setScene(std::unique_ptr<Scene> newScene) {
    if (!sceneStack.empty()) {
        sceneStack.back()->onExit();
        sceneStack.pop_back();
    }
    if (newScene) {
        sceneStack.push_back(std::move(newScene));
        sceneStack.back()->onInit();
    }
}

std::unique_ptr<Scene> Window::getTopScene() {
    if (!sceneStack.empty()) return std::move(sceneStack.back());
    return nullptr;
}

void Window::handleUpdate() {
    for (auto& scene : sceneStack) {
        if(sceneStack.back() != scene && scene->shouldPause()) {
            continue;
        }
        scene->onUpdate();
    }
}

void Window::handleRender() {

    float renderWidth = config.fixedCoordinateMode ? config.virtualSize.width : config.size.width;
    float renderHeight = config.fixedCoordinateMode ? config.virtualSize.height : config.size.height;
    float devicePixelRatio = 1.0f;

    if (config.fixedCoordinateMode) {
        devicePixelRatio = actualWindowSize.width / renderWidth;
    }
    
    for (auto& scene : sceneStack) {

        nvgBeginFrame(Renderer::context,
            renderWidth,
            renderHeight,
            devicePixelRatio);

        Renderer::save();
        Camera& cam = scene->getCamera();

        Renderer::translate(-cam.point);
        Renderer::scaleAndRotate(Rect(cam.point, renderWidth, renderHeight), cam.zoom, cam.angle);
        
        scene->onRender();
        Renderer::restore();

        nvgEndFrame(Renderer::context);
    }
}

void Window::handleMousePressed(Point point, int button) {
    if (!sceneStack.empty()) {
        sceneStack.back()->onMousePressed(point, button);
    }
}

void Window::handleMouseReleased(Point point, int button) {
    if (!sceneStack.empty()) {
        sceneStack.back()->onMouseReleased(point, button);
    }
}

void Window::handleMouseMoved(Point point) {
    if (!sceneStack.empty()) {
        sceneStack.back()->onMouseMoved(point);
    }
}

void Window::handleKeyPressed(int keyCode, int scanCode, int modifiers, bool repeat) {
    if (!sceneStack.empty()) {
        sceneStack.back()->onKeyPressed(keyCode, scanCode, modifiers, repeat);
    }
}

void Window::handleKeyReleased(int keyCode, int scanCode, int modifiers) {
    if (!sceneStack.empty()) {
        sceneStack.back()->onKeyReleased(keyCode, scanCode, modifiers);
    }
}

void Window::handleTextInput(const std::string& text) {
    if (!sceneStack.empty()) {
        sceneStack.back()->onTextInput(text);
    }
}

void Window::handleFocusGained() {
    for(auto& scene : sceneStack) {
        scene->onFocusGained();
    }
}

void Window::handleFocusLost() {
    for(auto& scene : sceneStack) {
        scene->onFocusLost();
    }
}

void Window::handleResize(int width, int height) {
    for(auto& scene : sceneStack) {
        scene->onResize(width, height);
    }
}