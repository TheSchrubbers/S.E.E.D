cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CONFIGURATION_TYPES Release CACHE STRING "" FORCE)

ENABLE_LANGUAGE(C)

project(SEED CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# OpenGL and GLUT package
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU X11)

file(
    GLOB_RECURSE
    GRAPHICS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Seed/Graphics/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Seed/Graphics/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Seed/Graphics/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Seed/Graphics/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ressources/*.hpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# GLM include directories
include_directories(${CMAKE_SOURCE_DIR}/lib-required/glm)
# include Materials
include_directories(${CMAKE_SOURCE_DIR}/include/Seed/Graphics/Materials)
# AntTweakBar include directories
include_directories(${CMAKE_SOURCE_DIR}/lib-required/antTweakBar/include)
# Boost include directories
include_directories(${CMAKE_SOURCE_DIR}/lib-required/boost_win32)
# Assimp include directories
include_directories(${CMAKE_SOURCE_DIR}/lib-required/assimp_win32/include)
# GLFW include directories
include_directories(${CMAKE_SOURCE_DIR}/lib-required/glfw_win32/include)
if(WIN32)
	# Glew include directories for windows
	include_directories(${CMAKE_SOURCE_DIR}/lib-required/glew_win32/include)
else(WIN32)
	find_package(GLEW REQUIRED)
	if(GLEW_FOUND)
		include_directories(${GLEW_INCLUDE_DIRS})
	endif(GLEW_FOUND)
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
    SHARED
    ${GRAPHICS_SRC}
)

if(WIN32)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/assimp_win32/lib/assimp_release-dll_win32/assimp.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glew_win32/lib/Release/Win32/glew32.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glfw_win32/lib-vc2013/glfw3.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glfw_win32/lib-vc2013/glfw3dll.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/antTweakBar/lib/AntTweakBar.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_filesystem-vc120-mt-1_59.lib)
	target_link_libraries(Seed-Graphics ${OPENGL_INCLUDE_DIRS})
else(WIN32)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/assimp/lib/libassimp.so.3.2.0)
	#target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/glew/lib/libGLEW.so.1.13.0)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/glfw/lib/libglfw.so.3.1)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/boost/lib/libboost_system.so.1.59.0)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/AntTweakBar/lib/libAntTweakBar.so.1.0)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/boost/lib/libboost_filesystem.so.1.59.0)
	target_link_libraries(Seed-Graphics ${OPENGL_INCLUDE_DIRS})
	if(GLEW_FOUND)
		target_link_libraries(Seed-Graphics ${GLEW_LIBRARIES})
	endif(GLEW_FOUND)
endif(WIN32)

#MAIN
get_filename_component(ProjectId ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/../examples/GraphicsTest/src/*.?pp)

add_executable(${ProjectId} ${SRC})

if(WIN32)
	target_link_libraries(${ProjectId} opengl32.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/lib-required/glfw_win32/lib-vc2013/glfw3.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/lib-required/glew_win32/lib/Release/Win32/glew32.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/lib-required/antTweakBar/lib/AntTweakBar.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/lib-required/assimp_win32/lib/assimp_release-dll_win32/assimp.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_system-vc120-mt-1_59.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_filesystem-vc120-mt-1_59.lib)
	target_link_libraries(${ProjectId} ${PROJECT_BINARY_DIR}/Release/Seed-Graphics.lib)
else(WIN32)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/assimp/lib/libassimp.so.3.2.0)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/glfw/lib/libglfw.so.3.1)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/boost/lib/libboost_system.so.1.59.0)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/boost/lib/libboost_filesystem.so.1.59.0)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/AntTweakBar/lib/libAntTweakBar.so.1.0)
	#target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/glew/lib/libGLEW.so.1.13.0)
	target_link_libraries(${ProjectId} ${PROJECT_BINARY_DIR}/lib/libSeed-Graphics.so)
	target_link_libraries(${ProjectId} ${OPENGL_INCLUDE_DIRS})
	if(GLEW_FOUND)
		target_link_libraries(${ProjectId} ${GLEW_LIBRARIES})
	endif(GLEW_FOUND)
endif(WIN32)

if(WIN32)
	#COPY FILES INTO THE BINARY PROJECT FILES
	file(GLOB DLL
		${CMAKE_SOURCE_DIR}/lib-required/DLL/*.dll
	)
	file(COPY ${DLL} DESTINATION ${PROJECT_BINARY_DIR}/bin/Release)
	file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${PROJECT_BINARY_DIR}/bin/Release)
else(WIN32)
	file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${PROJECT_BINARY_DIR}/bin)
endif(WIN32)
