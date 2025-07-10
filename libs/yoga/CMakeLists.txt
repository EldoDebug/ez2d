# Yoga: version 3.2.1

file(GLOB_RECURSE YOGA_SOURCES *.c *.cpp)
add_library(yoga STATIC ${YOGA_SOURCES})
target_include_directories(yoga PUBLIC ${CMAKE_CURRENT_LIST_DIR})