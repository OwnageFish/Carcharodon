# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set ( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR} )
set ( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib )

project ("Carcharodon")

# Include sub-projects.
add_subdirectory ("Carcharodon")
add_subdirectory ("BspProcessor")
add_subdirectory ("BspViewer")