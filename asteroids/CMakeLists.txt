cmake_minimum_required(VERSION 3.0)
project(asteroids C CXX)
add_executable(asteroids WIN32 asteroids.c icon.o)
target_link_libraries(asteroids winmm gdi32 opengl32 dsound)