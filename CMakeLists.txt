# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Carcharodon")

add_library($<TARGET_OBJECTS:BspProcessor>)

# Include sub-projects.
add_subdirectory ("Carcharodon")
add_subdirectory ("BspReader")
