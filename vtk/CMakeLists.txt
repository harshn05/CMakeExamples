cmake_minimum_required(VERSION 3.5)
project(EvoSim)
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_USE_RELATIVE_PATHS ON)
#GRABBING VTK
#find_package(VTK REQUIRED)
find_package(VTK
 COMPONENTS
    CommonColor
    CommonCore
    CommonDataModel
    FiltersCore
    FiltersSources
    IOImage
    ImagingHybrid
    InteractionWidgets
    RenderingCore
    RenderingVolumeOpenGL2
    #
    # These modules are suggested since they implement an existing module.
    # You may need to uncomment one or more of these.
    # If vtkRenderWindow is used and you want to use OpenGL,
    #   you also need the RenderingOpenGL2 module.
    # If vtkRenderWindowInteractor is used,
    #    uncomment RenderingUI and possibly InteractionStyle.
    # If text rendering is used, uncomment RenderingFreeType
    #
    # FiltersParallel   # implements VTK::FiltersCore
    InteractionStyle  # implements VTK::RenderingCore
    # RenderingCellGrid # implements VTK::RenderingCore
    # RenderingFreeType # implements VTK::RenderingCore
    RenderingOpenGL2  # implements VTK::RenderingCore
    RenderingUI       # implements VTK::RenderingCore
)
add_executable(MAIN main.cpp)
target_link_libraries(MAIN ${VTK_LIBRARIES})
#vtk_module_autoinit(TARGETS MAIN MODULES ${VTK_LIBRARIES})
