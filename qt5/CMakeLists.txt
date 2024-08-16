cmake_minimum_required(VERSION 3.0)

project(Minimal_Qt6_CMAKE_Example)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_USE_RELATIVE_PATHS ON)
find_package(Qt6Widgets REQUIRED QUIET)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

option(Separate_DLL_For_GUI "Create Separate DLL for GUI" ON)
file(GLOB uiFiles "*.ui")
qt6_wrap_ui(UISrcs ${uiFiles})

if(Separate_DLL_For_GUI)
	add_library(GUI UiWrapper.hpp UiWrapper.cpp FromDesigner.ui)
	target_link_libraries(GUI Qt6::Core Qt6::Gui Qt6::Widgets)
	add_executable(App ApplicationDriver.cpp)
	target_link_libraries(App GUI)
else()
	file(GLOB allFiles "*.hpp" "*.cpp"  "*.h" "*.ui" "*.qrc")
	add_executable(App ${uiFiles} ${allFiles})
	target_link_libraries(App Qt6::Core Qt6::Gui Qt6::Widgets)
endif(Separate_DLL_For_GUI)





