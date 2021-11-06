# CMakeList.txt : CMake project for Carcharodon, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


target_link_libraries(BspReader)
# Add BspReader object library?
#add_library ($<TARGET_OBJECTS:BspReader>)

# Add source to this project's executable.
add_executable (Carcharodon "Carcharodon.cpp" "Carcharodon.h" ${BspR)

# TODO: Add tests and install targets if needed.
