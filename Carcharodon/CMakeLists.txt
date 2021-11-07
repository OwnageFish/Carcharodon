# CMakeList.txt : CMake project for Carcharodon, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
	"."
	"${PROJECT_SOURCE_DIR}/BspProcessor"
)

# Add BspReader object library?
#add_library ($<TARGET_OBJECTS:BspReader>)

link_directories ( "${LIBRARY_OUTPUT_PATH}" )

# Add source to this project's executable.
add_executable (Carcharodon "Carcharodon.cpp")

#add_dependencies (Carcharodon
#	BspProcessor
#)

target_link_libraries (Carcharodon
	BspProcessor
)

# TODO: Add tests and install targets if needed.
