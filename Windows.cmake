# Specify your custom search paths for Windows platforms
if(WIN32)
	# A folder where you are keeping all of your libs/includes.
	set(WINDOWS_LIBRARY_PATH "D:/Env/Libraries/")
	
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