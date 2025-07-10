add_library(rtaudio STATIC RtAudio.cpp)
target_include_directories(rtaudio PUBLIC ${CMAKE_CURRENT_LIST_DIR})