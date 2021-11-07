# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Carcharodon")

set ( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR} )
set ( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib )

# Include sub-projects.
add_subdirectory ("Carcharodon")
add_subdirectory ("BspProcessor")