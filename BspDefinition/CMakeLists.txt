# CMakeList.txt : CMake project for BspDefinition, include source and define
# project specific logic here.
#

cmake_minimum_required (VERSION 3.8)

include_directories (
	"."
)

add_library (BspDefinition
	"BSP.h"
)