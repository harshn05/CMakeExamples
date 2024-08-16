cmake_minimum_required(VERSION 3.5)
project(EvoSim)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_USE_RELATIVE_PATHS ON)

#GRABBING OMP
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  ENDIF()
ENDIF()

find_package(Qt6Widgets REQUIRED QUIET)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)
file(GLOB uiFiles "*.ui")
qt6_wrap_ui(UISrcs ${uiFiles})

#GRABBING VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_library(AuxFuncs SHARED AuxFuncs.hpp AuxFuncs.cpp)

add_library(MTEngine SHARED MTEngine.hpp MTEngine.cpp)

add_library(Matrix SHARED Matrix.hpp Matrix.cpp)
target_link_libraries(Matrix AuxFuncs)



add_library(Visualization SHARED Visualization.hpp Visualization.cpp)
target_link_libraries(Visualization ${VTK_LIBRARIES} Matrix MTEngine)
vtk_module_autoinit(TARGETS Visualization MODULES ${VTK_LIBRARIES})

add_library(Domain SHARED Domain.hpp Domain.cpp)
target_link_libraries(Domain Matrix Visualization)

add_library(GUI SHARED UiWrapper.hpp UiWrapper.cpp FromDesigner.ui)
target_link_libraries(GUI Qt6::Core Qt6::Gui Qt6::Widgets Visualization Domain)

add_executable(App ApplicationDriver.cpp)
target_link_libraries(App GUI)