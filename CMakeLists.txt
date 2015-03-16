cmake_minimum_required(VERSION 2.6)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project("TextEngine" C CXX)  # This project uses C and C++
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wextra -g -Wno-unused-parameter -Wno-unused-function")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Wall -g")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules)

# use color messages
#include(${CMAKE_SOURCE_DIR}/CMakeModules/colorMessage.cmake)

# Add names to this list to make your own projects!
set(BUILD_LIST main)

if(WIN32)
	# Specify your custom search paths for Windows platforms
	
	# A folder where you are keeping all of your libs/includes.
	set(WINDOWS_LIBRARY_PATH "D:/Env/Libraries/")
	
	# MinGW installation path.
	#set(MINGW_ROOT_PATH "D:/Env/MinGW/bin/")
	
	#set(CMAKE_C_COMPILER "${MINGW_ROOT_PATH}gcc.exe")
	#set(CMAKE_CXX_COMPILER "${MINGW_ROOT_PATH}g++.exe")

	# Specify GLUT installation
	set(GLUT_ROOT_PATH "${WINDOWS_LIBRARY_PATH}freeglut/" ${GLUT_ROOT_PATH})
	
	# I want to give this a try some day
	set(GLFW_LOCATION "${WINDOWS_LIBRARY_PATH}glfw-3.1" ${GLUT_ROOT_PATH})
	set(CMAKE_LIBRARY_PATH "${GLFW_LOCATION}/src" ${CMAKE_LIBRARY_PATH})
	set(CMAKE_INCLUDE_PATH "${GLFW_LOCATION}/include" ${CMAKE_INCLUDE_PATH})
	
	# Specify GLEW installation
	set(GLEW_ROOT_PATH "${WINDOWS_LIBRARY_PATH}glew-1.12.0/")
	set(CMAKE_LIBRARY_PATH "${GLEW_ROOT_PATH}lib/Release/Win32" ${CMAKE_LIBRARY_PATH})
	set(CMAKE_INCLUDE_PATH "${GLEW_ROOT_PATH}include" ${CMAKE_INCLUDE_PATH})
	
	# Specify FreeType installation
	set(FREETYPE_ROOT_PATH "${WINDOWS_LIBRARY_PATH}freetype-2.5.5/")
	set(CMAKE_LIBRARY_PATH "${FREETYPE_ROOT_PATH}objs" ${CMAKE_LIBRARY_PATH})
	set(CMAKE_INCLUDE_PATH "${FREETYPE_ROOT_PATH}include" ${CMAKE_INCLUDE_PATH})
endif()

# Use glxinfo or glewinfo to determine OpenGL version if they are
# available. Fail if a high enough version of OpenGL is not supported.
find_file(GLXINFO glxinfo)
find_file(GLEWINFO glewinfo)
if(UNIX AND NOT APPLE AND (GLXINFO OR GLEWINFO))
	execute_process(COMMAND ${GLXINFO} COMMAND grep "OpenGL core profile version string" COMMAND cut -d " " -f 6 OUTPUT_VARIABLE openglversion OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
	if(NOT openglversion AND GLEWINFO)
		execute_process(COMMAND ${GLEWINFO} COMMAND grep "OpenGL version " COMMAND cut -d " " -f 3 OUTPUT_VARIABLE openglversion OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
	endif()

	if(openglversion)
		message("OpenGL core profile version - ${openglversion}")
		if(openglversion VERSION_LESS "3.2")
			message(FATAL_ERROR "OpenGL version 3.2 or higher is required.")
		endif()
	endif()
endif()


# SITE_NAME sets the HOSTNAME variable to the name of this computer.
SITE_NAME(HOSTNAME)


# Doxygen documentation
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" ON)
if(BUILD_DOCUMENTATION)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
	#-- Add a custom target to run Doxygen when ever the project is built
	add_custom_target (docs 
                     COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                     SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
	# Add or remove 'ALL' after 'docs' above to enable/disable doxygen generation when running "make"
	else()
		message(WARNING "Not creating Doxygen documents because doxygen was not found on this system.")
	endif()
endif()


# === Libraries that we use ===
# --- math library ---
if(NOT WIN32)
  find_library(M_LIBRARY
    NAMES m
    PATHS /usr/lib /usr/local/lib
  )
  if(NOT M_LIBRARY)
    message(STATUS
      "math library 'libm' not found - floating point support disabled")
  endif()
else()
  # not needed on windows
  set(M_LIBRARY "")
endif()

# --- OpenGL ---
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# --- Freeglut (version 2.6 or higher) ---
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

# Make sure freeglut installation is new enough!
include(CheckSymbolExists)
if(UNIX AND NOT APPLE)
  CHECK_SYMBOL_EXISTS(GLUT_CORE_PROFILE "${GLUT_INCLUDE_DIR}/GL/freeglut.h;${GLUT_INCLUDE_DIR}/GL/freeglut_ext.h" FREEGLUT_NEW_ENOUGH)
  if(NOT FREEGLUT_NEW_ENOUGH)
    message(FATAL_ERROR "freeglut version 2.6 or higher is required.")
  endif()
endif()

# --- GLEW ---
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

# Check for an newer extension that we use for glGetProgram(). If GLEW
# is too old, it might not know about the extension and we could get
# compile errors.
include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(GL_ARB_get_program_binary "${GLEW_INCLUDE_DIRS}/GL/glew.h" GLEW_NEW_ENOUGH)
if(NOT GLEW_NEW_ENOUGH)
  message(FATAL_ERROR "glew version 1.5.6 or higher is required.")
endif()

# --- FreeType ---
find_package(FreeType REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Collect libraries that all programs use together into one cmake variable
set(COMMON_LIBRARIES ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES} ${M_LIB} ${GLUT_LIBRARIES})
set(COMMON_SOURCES "")

# Create a static library containing all files that we regularly use.
include_directories("include")
set(FILES_IN_ENGINE src/CFont.cpp src/CCharacter.cpp src/CTextBuffer.cpp src/CCanvas.cpp src/CGame.cpp)
add_library(TextEngine STATIC ${FILES_IN_ENGINE})
set_target_properties(TextEngine PROPERTIES COMPILE_DEFINITIONS "${PREPROC_DEFINE}")

# Make compilation rule for all programs that don't need ImageMagick or ASSIMIP
foreach(arg ${BUILD_LIST})
	add_executable(${arg} ${arg}.cpp)
	target_link_libraries(${arg} TextEngine ${COMMON_LIBRARIES})
	message(${COMMON_LIBRARIES})
	set_target_properties(${arg} PROPERTIES COMPILE_DEFINITIONS "MOUSEMOVE_GLUT;${MISSING_VRPN_DEFINITION}")
endforeach()

