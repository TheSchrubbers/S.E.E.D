cmake_minimum_required(VERSION 2.8)

project(SEED CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

file(
    GLOB_RECURSE
    GRAPHICS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Seed/Graphics/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Seed/Graphics/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ressources/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/Seed/Graphics/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Seed/Graphics/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ressources/*.hpp
)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# GLM include directories
include_directories(lib-required/glm)
# CLMG include directories
include_directories(lib-required/clmg)
# include Materials
include_directories(ressources/Materials)
# AntTweakBar include directories
include_directories(lib-required/antTweakBar/include)
# Boost include directories
include_directories(lib-required/boost_win32)

if(WIN32)

	# Assimp include directories for windows
	include_directories(lib-required/assimp_win32/include)

	# Glew include directories for windows
	include_directories(lib-required/glew_win32/include)

	# GLFW include directories for windows
	include_directories(lib-required/glfw_win32/include)

else(WIN32)

	# Assimp include directories for linux
	include_directories(lib-required/assimp_linux/include)

	# Glew include directories for linux
	include_directories(lib-required/glew_linux/include)

	# GLFW include directories for linux
	include_directories(lib-required/glfw_linux/include)

endif(WIN32)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in 
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY
	)

	add_custom_target(
		doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)

#Seed's graphics library
add_library(
    Seed-Graphics

    STATIC
    
    ${GRAPHICS_SRC}
)

if(WIN32)
	find_package(OpenGL REQUIRED)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/assimp_win32/lib/assimp_release-dll_win32/assimp.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glew_win32/lib/Release/Win32/glew32.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glfw_win32/lib-vc2013/glfw3.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glfw_win32/lib-vc2013/glfw3dll.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/antTweakBar/lib/AntTweakBar.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_filesystem-vc120-mt-gd-1_59.lib)
	target_link_libraries(Seed-Graphics ${OPENGL_INCLUDE_DIRS})

else(WIN32)

	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/assimp_linux/lib/libassimp.so.3)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glew_linux/lib/libGLEW.so.1.13)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glfw_linux/lib/libglfw.so.3.1)

endif(WIN32)

#All Seed's libraries
#add_library(
#    Seed-All
    
# 	STATIC
    
#	${GRAPHICS_SRC}
#)
