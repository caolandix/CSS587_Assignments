# CMakeList.txt : CMake project for CSS587_Template, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 4.2.3 FATAL_ERROR)
project(CSS587_Assignments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenCV_DIRBASE "C:/Users/Caolan/Documents")
set(OpenCV_DIR "C:/Users/Caolan/Documents/opencv_4.x-4.x/out/build/x64-Debug")
find_package(OpenCV REQUIRED COMPONENTS
  core
  highgui
  imgproc
  imgcodecs
)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Add source to this project's executable.
add_executable (CSS587_Assignments 
	"CSS587_Assignments.cpp" "CSS587_Assignments.h"
)

target_include_directories(CSS587_Assignments PRIVATE 
	${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(CSS587_Assignments PRIVATE
  ${OpenCV_LIBS}
)

include(GNUInstallDirs)
install(TARGETS CSS587_Assignments
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# TODO: Add tests and install targets if needed.
