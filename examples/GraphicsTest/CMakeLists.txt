cmake_minimum_required(VERSION 2.6)

get_filename_component(ProjectId ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SEED_INCLUDE "../../seed-framework/include/")
set(SEED_GRAPHICS_LIB ${CMAKE_SOURCE_DIR}/../../seed-framework/build/lib/libSeed-Graphics.so)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include_directories(${SEED_INCLUDE})

# GLM include directory
include_directories(${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glm)

# OpenGL and GLUT package
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU X11)

include_directories(${OPENGL_INCLUDE_DIRS})

if(WIN32)

	# Assimp include directories for windows
	include_directories(${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/assimp_win/include)
	set(ASSIMP_LIB ${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/assimp_win/lib/libassimp.so.3)

	# Glew include directories for windows
	include_directories(${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glew_win32/include)
	set(GLEW_LIB ${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glew_win32/lib/libGLEW.so.1.13)

	# GLFW include directories for windows
	include_directories(${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glfw_win32/include)
	set(GLFW_LIB ${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glfw_linux/lib-vc2013/glfw3.lib)

else(WIN32)

	# Assimp include directories for linux
	include_directories(${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/assimp_linux/include)
	set(ASSIMP_LIB ${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/assimp_linux/lib/libassimp.so.3)

	# Glew include directories for assimp_linux
	include_directories(${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glew_linux/include)
	set(GLEW_LIB ${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glew_linux/lib/libGLEW.so.1.13)

	# GLFW include directories for linux
	include_directories(${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glfw_linux/include)
	set(GLFW_LIB ${CMAKE_SOURCE_DIR}/../../seed-framework/lib-required/glfw_linux/lib/libglfw.so.3.1)

endif(WIN32)

file(GLOB_RECURSE SRC src/*.?pp)

add_executable(${ProjectId} ${SRC})

target_link_libraries(${ProjectId} ${ASSIMP_LIB})
target_link_libraries(${ProjectId} ${GLEW_LIB})
target_link_libraries(${ProjectId} ${GLFW_LIB} ${GL_LIBRARY} m)
target_link_libraries(${ProjectId} ${SEED_GRAPHICS_LIB})