include(FetchContent)
cmake_minimum_required(VERSION 3.10.0)
project(ez2d VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage(
    NAME SDL3
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-3.2.16
    OPTIONS
        "SDL_TEST OFF"
        "SDL_SHARED OFF"
        "SDL_STATIC ON"
)

CPMAddPackage(
    NAME bgfx
    GITHUB_REPOSITORY bkaradzic/bgfx.cmake
    GIT_TAG v1.129.8914-493
    OPTIONS
        "BGFX_BUILD_TOOLS ON"
        "BGFX_BUILD_TOOLS_SHADER ON"
        "BGFX_BUILD_TOOLS_TEXTURE ON"
        "BGFX_BUILD_TOOLS_BIN2C ON"
        "BGFX_BUILD_TOOLS_GEOMETRY ON"
        "BGFX_INSTALL ON"
        "BGFX_BUILD_EXAMPLES OFF"
        "BGFX_BUILD_TESTS OFF"
)

CPMAddPackage(
    NAME libsndfile
    GITHUB_REPOSITORY libsndfile/libsndfile
    GIT_TAG master
    OPTIONS
        "BUILD_EXAMPLES OFF"
        "BUILD_TESTING OFF"
)

CPMAddPackage(
    NAME box2d
    GITHUB_REPOSITORY erincatto/box2d
    GIT_TAG v3.1.1
    OPTIONS
        "BOX2D_BUILD_TESTS OFF"
        "BOX2D_BUILD_EXAMPLES OFF"
        "BOX2D_BUILD_DOCS OFF"
)

add_subdirectory(libs/stb)
add_subdirectory(libs/nanovg)
add_subdirectory(libs/rtaudio)
add_subdirectory(libs/yoga)
add_subdirectory(example)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.c"
)

list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(ez2d STATIC ${LIB_SOURCES})

target_link_libraries(
    ez2d
    PUBLIC
    SDL3::SDL3
    bgfx
    bimg
    bx
    nanovg
    box2d
    yoga
    rtaudio
    sndfile
)

target_include_directories(ez2d PUBLIC src)

add_executable(ez2d_executable src/main.cpp)

target_link_libraries(
    ez2d_executable
    ez2d
    example
)