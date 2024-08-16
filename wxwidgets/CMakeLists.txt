project(EvoSim)
cmake_minimum_required(VERSION 2.8)
set(wxWidgets_CONFIGURATION mswu)
SET(wxWidgets_USE_LIBS)
find_package(wxWidgets)
#find_package(wxWidgets COMPONENTS mono)

include( "${wxWidgets_USE_FILE}" )

file(GLOB SRC
	 "*.h"
	"*.cpp"
)

set(SRCS ${SRCS} resource.rc)
add_executable(
    ${PROJECT_NAME} WIN32
    ${SRC}
    )

target_link_libraries(
    ${PROJECT_NAME}
    ${wxWidgets_LIBRARIES}
    )
