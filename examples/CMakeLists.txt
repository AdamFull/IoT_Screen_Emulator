cmake_minimum_required(VERSION 3.2)
project(Embeeded_GUI_API CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/)

file(GLOB CPP_FILES *.cpp *.c *.h)
add_executable(Embeeded_GUI_API ${CPP_FILES})

target_link_libraries(Embeeded_GUI_API IOT_SCREEN_EMULATOR)