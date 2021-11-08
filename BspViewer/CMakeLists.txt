# CMakeList.txt : CMake project for BspViewer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
	"."
	"${PROJECT_SOURCE_DIR}/BspProcessor"
)

link_directories ( "${LIBRARY_OUTPUT_PATH}" )

add_library (BspViewer
	"BspViewer.cpp"
)

target_link_libraries (BspViewer
	BspProcessor
)