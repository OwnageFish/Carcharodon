cmake_minimum_required (VERSION 3.8)

find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)

message(STATUS "GLM included at ${OpenGL_INCLUDE_DIR}")

find_library(SDL2_LIBRARY REQUIRED NAMES SDL2 PATHS "${PROJECT_SOURCE_DIR}/ExtrnDll/SDL")
find_library(SDL2Main_LIBRARY REQUIRED NAMES SDL2main PATHS "${PROJECT_SOURCE_DIR}/ExtrnDll/SDL")
find_library(GLEW_LIBRARY REQUIRED NAMES glew32 PATHS "${PROJECT_SOURCE_DIR}/ExtrnDll/Glew")
# find_library(GLEW_LIBRARY REQUIRED NAMES glew32s PATHS "${PROJECT_SOURCE_DIR}/ExtrnDll/Glew")
message(STATUS "GLM included at ${GLEW_LIBRARY}")

include_directories (
	"."
	"./ViewerUtilities"
	"${PROJECT_SOURCE_DIR}/BspProcessor"
	"${PROJECT_SOURCE_DIR}/ExtrnDll/SDL/include"
	"${PROJECT_SOURCE_DIR}/ExtrnDll/Glew/include"
	"${PROJECT_SOURCE_DIR}/ExtrnDll/GLM/include"
)

add_executable (BspViewer WIN32 "BspViewer.cpp")

# target_compile_options(BspViewer PUBLIC "/std:c++latest")

target_link_libraries (BspViewer
	BspProcessor
	"${SDL2_LIBRARY}"
	"${SDL2Main_LIBRARY}"
	opengl32
	"${GLEW_LIBRARY}"
)

# Copy DLLs
file(GLOB DLL_LIBRARIES
	"${PROJECT_SOURCE_DIR}/ExtrnDll/SDL/*.dll"  #SDL2_LIB_DLLS
	"${PROJECT_SOURCE_DIR}/ExtrnDll/Glew/*.dll" #GLEW_LIBS
)
file(COPY ${DLL_LIBRARIES}
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# Copy shaders
file(GLOB SHADER_SOURCES
	"${PROJECT_SOURCE_DIR}/BspViewer/Shaders/*.glsl"  #SDL2_LIB_DLLS
)
file(COPY ${SHADER_SOURCES}
	 DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Shaders"
)

# Copy example BSP
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/de_shortnuke.bsp
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
# Copy example BSP
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/de_shortdust.bsp
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# Let CMake make use of various external headers
add_subdirectory (ViewerUtilities)