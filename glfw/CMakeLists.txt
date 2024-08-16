cmake_minimum_required(VERSION 3.0)
project(TEST)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(GDI32)

add_executable(test main.cpp)
target_link_libraries(test glfw OpenGL::GL)