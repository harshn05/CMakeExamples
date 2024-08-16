cmake_minimum_required(VERSION 3.10)

# set the project name
project(TEST)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "X:/C++/BuildToolsMSVC2022/opt/Armadillo/share/Armadillo/CMake")
find_package(BLAS)
# add the executable
add_executable(test main.cpp)
find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
target_link_libraries(test ${ARMADILLO_LIBRARIES} ${BLAS_LIBRARIES} gfortran quadmath)
