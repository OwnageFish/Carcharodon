#set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
#set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/SDL2main.lib;${CMAKE_CURRENT_LIST_DIR}/SDL2image.lib")
#find_library(SDL2_LIB NAMES SDL2.lib PATHS "${CMAKE_CURRENT_LIST_DIR}")
#find_library(SDL2_MAIN_LIB NAMES SDL2main.lib PATHS "${CMAKE_CURRENT_LIST_DIR}")
#string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

# Support both 32 and 64 bit builds
# if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
#  set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2main.lib")
# else ()
#  set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2main.lib")
# endif ()

#add_library(
#	sdl2_lib STATIC IMPORTED
#)

#set_target_properties ( 
#	sdl2_lib PROPERTIES 
#	IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/ExtrnDll/SDL/SDL2.lib"
#)

#add_library(
#	sdl2_lib_main STATIC IMPORTED
#)

#set_target_properties ( 
#	sdl2_lib_main PROPERTIES 
#	IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/ExtrnDll/SDL/SDL2main.lib"
#)