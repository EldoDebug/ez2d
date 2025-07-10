file(GLOB STB_SOURCES *.c)
add_library(stb STATIC ${STB_SOURCES})
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(stb PUBLIC bgfx bx bimg)