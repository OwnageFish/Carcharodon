﻿# CMakeList.txt : CMake project for Carcharodon, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
	"."
#	"${PROJECT_SOURCE_DIR}/BspViewer"
#	"${PROJECT_SOURCE_DIR}/BspDefinition"
	"${PROJECT_SOURCE_DIR}/BspProcessor"
)

# Add BspReader object library?
link_directories ( "${LIBRARY_OUTPUT_PATH}" )

# Add source to this project's executable.
add_executable (Carcharodon "Carcharodon.cpp")

add_dependencies (Carcharodon
	BspProcessor
#	BspViewer
#	BspDefinition
)

target_link_libraries (Carcharodon
	BspProcessor
#	BspViewer
#	BspDefinition
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/example.bsp
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# TODO: Add tests and install targets if needed.
