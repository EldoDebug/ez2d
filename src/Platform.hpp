#pragma once
#include <SDL3/SDL.h>

#if defined (SDL_PLATFORM_LINUX)
    #define LINUX
  #if __has_include(<wayland-client.h>)
    #define LINUX_WAYLAND
  #elif __has_include(<X11/Xlib.h>)
    #define LINUX_X11
  #endif
#endif

#if defined (SDL_PLATFORM_MACOS)
    #define MACOS
#endif

#if defined (SDL_PLATFORM_WIN32)
    #define WINDOWS
#endif