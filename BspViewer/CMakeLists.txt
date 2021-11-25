cmake_minimum_required (VERSION 3.8)

find_package(OpenGL REQUIRED)

message(STATUS "GLM included at ${OpenGL_INCLUDE_DIR}")

find_library(SDL2_LIBRARY REQUIRED NAMES SDL2 PATHS "${PROJECT_SOURCE_DIR}/ExtrnDll/SDL")
find_library(SDL2Main_LIBRARY REQUIRED NAMES SDL2main PATHS "${PROJECT_SOURCE_DIR}/ExtrnDll/SDL")
find_library(GLEW_LIBRARY REQUIRED NAMES glew32s PATHS "${PROJECT_SOURCE_DIR}/ExtrnDll/Glew")

include_directories (
	"."
	"./ViewerUtilities"
	"${PROJECT_SOURCE_DIR}/BspProcessor"
	"${PROJECT_SOURCE_DIR}/ExtrnDll/SDL/include"
	"${PROJECT_SOURCE_DIR}/ExtrnDll/Glew/include"
	"${PROJECT_SOURCE_DIR}/ExtrnDll/GLM/include"
)

add_executable (BspViewer WIN32 "BspViewer.cpp")

target_link_libraries (BspViewer
	BspProcessor
	"${SDL2_LIBRARY}"
	"${SDL2Main_LIBRARY}"
	"${GLEW_LIBRARY}"
	opengl32
)

file(GLOB SDL2_LIB_DLLS
	"${PROJECT_SOURCE_DIR}/ExtrnDll/SDL/*.dll"
)
file(COPY ${SDL2_LIB_DLLS}
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# Let CMake make use of various external headers
add_subdirectory (ViewerUtilities)