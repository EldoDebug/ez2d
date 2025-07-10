file(GLOB_RECURSE EXAMPLE_SOURCES *.c *.cpp)
add_library(example STATIC ${EXAMPLE_SOURCES})
target_include_directories(example PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(example PUBLIC ez2d)