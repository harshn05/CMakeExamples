cmake_minimum_required(VERSION 3.5)
project(Sample)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#FOR OPENGL
find_package(OpenGL REQUIRED)

###############External Projects######################
#FOR GLFW
include(ExternalProject)
ExternalProject_Add(
  GLFW_LIB
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/glfw"  
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}  -DCMAKE_BUILD_TYPE=Release -DGLFW_BUILD_TESTS=FALSE -DGLFW_BUILD_EXAMPLES=FALSE -DGLFW_BUILD_DOCS=FALSE
)

#FOR FREEGLUT
ExternalProject_Add(
  FREEGLUT_LIB
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/freeglut"  
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}  -DCMAKE_BUILD_TYPE=Release -DFREEGLUT_BUILD_DEMOS=FALSE -DFREEGLUT_BUILD_SHARED_LIBS=FALSE
)


#FOR GLEW
ExternalProject_Add(
  GLEW_LIB
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/glew"
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}  -DCMAKE_BUILD_TYPE=Release -Dglew-cmake_BUILD_SHARED=False -DONLY_LIBS=TRUE
)

include_directories(${CMAKE_BINARY_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
include_directories(${IMGUI_DIR}  ${IMGUI_DIR}/backends)

file(GLOB IMGUI_SRC 
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/*.h")
    
set(IMGUI_BACKEND_SRC
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

add_library(imgui ${IMGUI_SRC} ${IMGUI_BACKEND_SRC})


#FOR GLM
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)
include_directories(${GLM_DIR})

#FOR STB
set(STB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb)
include_directories(${STB_DIR})


#####################

#EXECUTABLE
file(GLOB src ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(main ${src})

add_dependencies(main GLFW_LIB GLEW_LIB)
target_link_libraries(main glew glfw3 imgui OpenGL::GL ${CMAKE_DL_LIBS})


