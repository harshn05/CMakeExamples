cmake_minimum_required(VERSION 3.0)

find_package(Qt6Widgets REQUIRED QUIET)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB uiFiles "*.ui")
qt6_wrap_ui(UISrcs ${uiFiles})
file(GLOB allFiles "*.hpp" "*.cpp"  "*.h" "*.ui" "*.qrc")
add_executable(App WIN32 ${uiFiles} ${allFiles})
target_link_libraries(App Qt6::Core Qt6::Gui Qt6::Widgets ${OpenCV_LIBS})




