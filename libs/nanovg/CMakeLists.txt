file(GLOB NANOVG_SOURCES *.cpp)
add_library(nanovg STATIC ${NANOVG_SOURCES})
target_include_directories(nanovg PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(nanovg PUBLIC bgfx bx bimg stb)