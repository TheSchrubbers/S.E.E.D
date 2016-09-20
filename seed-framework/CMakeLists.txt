cmake_minimum_required(VERSION 2.8.11)
SET(CMAKE_CONFIGURATION_TYPES Release CACHE STRING "" FORCE)

ENABLE_LANGUAGE(C)

project(SEED CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/graphics-engine/lib)

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

set(INCLUDES_DIR ${CMAKE_SOURCE_DIR}/lib-required/includes)
set(ASSIMP_INCLUDE_DIR ${INCLUDES_DIR})
set(ANTTWEAKBAR_INCLUDE_DIR ${INCLUDES_DIR}/antTweakBar)
set(BOOST_INCLUDE_DIR ${INCLUDES_DIR})
set(GLFW_INCLUDE_DIR ${INCLUDES_DIR}/glfw)
set(GLM_INCLUDE_DIR ${INCLUDES_DIR}/glm)
set(GLEW_INCLUDE_DIR ${INCLUDES_DIR}/glew)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/lib-required/libs)
set(ASSIMP_LIB_LINUX ${LIBS_DIR}/Linux/assimp/libassimp.so.3)
set(GLFW_LIB_LINUX ${LIBS_DIR}/Linux/glfw/libglfw.so.3.1)
set(BOOST_SYSTEM_LIB_LINUX ${LIBS_DIR}/Linux/boost/system/build/gcc-5.4.0/release/link-static/threading-multi/libboost_system.so.1.61.0)
set(BOOST_FILESYSTEM_LIB_LINUX ${LIBS_DIR}/Linux/boost/filesystem/build/gcc-5.4.0/release/link-static/threading-multi/libboost_filesystem.so.1.61.0)
set(Boost_USE_MULTITHREADED ON)
set(GLEW_LIB_LINUX ${LIBS_DIR}/Linux/glew/libGLEW.so.1.13.0)
set(ANTTWEAKBAR_LIB_LINUX ${LIBS_DIR}/Linux/AntTweakBar/libAntTweakBar.a)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# GLM include directories
include_directories(${GLM_INCLUDE_DIR})
# include Materials
include_directories(${CMAKE_SOURCE_DIR}/include/Seed/Graphics/Materials)
# AntTweakBar include directories
include_directories(${ANTTWEAKBAR_INCLUDE_DIR})
# Boost include directories
#include_directories(${BOOST_INCLUDE_DIR})
# Assimp include directories
include_directories(${ASSIMP_INCLUDE_DIR})
# GLFW include directories
include_directories(${GLFW_INCLUDE_DIR})

if(WIN32)
	# Glew include directories for windows
	include_directories(${GLEW_INCLUDE_DIR})
else(WIN32)
	find_package(GLEW REQUIRED)
	if(GLEW_FOUND)
		include_directories(${GLEW_INCLUDE_DIRS})
	else(GLEW_FOUND)
		include_directories(${GLEW_INCLUDE_DIR})
	endif(GLEW_FOUND)
	SET(CMAKE_INCLUDE_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")
	SET(CMAKE_LIBRARY_PATH ${CMAKE_INCLUDE_PATH} "/usr/local/include/boost")
	find_package(Boost 1.36.0 COMPONENTS system filesystem)
	if(Boost_FOUND)
		include_directories(${Boost_INCLUDE_DIRS})
	else(Boost_FOUND)
		include_directories(${BOOST_INCLUDE_DIR})
	endif(Boost_FOUND)
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
if(WIN32)
	add_library(
   	Seed-Graphics
    	STATIC
    	${GRAPHICS_SRC}
	)
else(WIN32)
	add_library(
   	Seed-Graphics
    	SHARED
    	${GRAPHICS_SRC}
	)
endif(WIN32)

if(WIN32)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/assimp_win32/lib/assimp_release-dll_win32/assimp.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glew_win32/lib/Release/Win32/glew32.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glfw_win32/lib-vc2013/glfw3.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/glfw_win32/lib-vc2013/glfw3dll.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/antTweakBar/lib/AntTweakBar.lib)
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_filesystem-vc120-mt-1_59.lib)
	target_link_libraries(Seed-Graphics ${OPENGL_INCLUDE_DIRS})
else(WIN32)
	target_link_libraries(Seed-Graphics ${ASSIMP_LIB_LINUX})
	target_link_libraries(Seed-Graphics ${GLFW_LIB_LINUX})
	target_link_libraries(Seed-Graphics ${ANTTWEAKBAR_LIB_LINUX})
	#target_link_libraries(Seed-Graphics ${BOOST_FILESYSTEM_LIB_LINUX})
	#target_link_libraries(Seed-Graphics ${BOOST_SYSTEM_LINUX})
	target_link_libraries(Seed-Graphics ${OPENGL_INCLUDE_DIRS})
	if(GLEW_FOUND)
		target_link_libraries(Seed-Graphics ${GLEW_LIBRARIES})
	else(GLEW_FOUND)
		target_link_libraries(Seed-Graphics ${GLEW_LIB_LINUX})
	endif(GLEW_FOUND)
	if(Boost_FOUND)
		target_link_libraries(Seed-Graphics ${Boost_LIBRARIES})
	else(Boost_FOUND)
		target_link_libraries(Seed-Graphics ${BOOST_SYSTEM_LINUX})
		target_link_libraries(Seed-Graphics ${BOOST_FILESYSTEM_LIB_LINUX})
	endif(Boost_FOUND)
endif(WIN32)

#MAIN
get_filename_component(ProjectId ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/graphics-engine/examples/bin)

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
	target_link_libraries(${ProjectId} ${PROJECT_BINARY_DIR}/graphics-engine/Release/Seed-Graphics.lib)
else(WIN32)
	target_link_libraries(${ProjectId} ${ASSIMP_LIB_LINUX})
	target_link_libraries(${ProjectId} ${GLFW_LIB_LINUX})
	target_link_libraries(${ProjectId} ${ANTTWEAKBAR_LIB_LINUX})
	#target_link_libraries(${ProjectId} ${BOOST_FILESYSTEM_LIB_LINUX})
	#target_link_libraries(${ProjectId} ${BOOST_SYSTEM_LINUX})
	target_link_libraries(${ProjectId} ${PROJECT_BINARY_DIR}/graphics-engine/lib/libSeed-Graphics.so)
	target_link_libraries(${ProjectId} ${OPENGL_INCLUDE_DIRS})
	if(GLEW_FOUND)
		target_link_libraries(${ProjectId} ${GLEW_LIBRARIES})
	else(GLEW_FOUND)
		target_link_libraries(${ProjectId} ${GLEW_LIB_LINUX})
	endif(GLEW_FOUND)
	if(Boost_FOUND)
		target_link_libraries(${ProjectId} ${Boost_LIBRARIES})
	else(Boost_FOUND)
	target_link_libraries(${ProjectId} ${BOOST_FILESYSTEM_LIB_LINUX})
		target_link_libraries(${ProjectId} ${BOOST_SYSTEM_LINUX})
	endif(Boost_FOUND)
endif(WIN32)

if(WIN32)
	#COPY FILES INTO THE BINARY PROJECT FILES
	file(GLOB DLL
		${CMAKE_SOURCE_DIR}/lib-required/DLL/*.dll
	)
	file(COPY ${DLL} DESTINATION ${PROJECT_BINARY_DIR}/graphics-engine/examples/bin/Release)
	file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${PROJECT_BINARY_DIR}/graphics-engine/examples/bin/Release)
else(WIN32)
	file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${PROJECT_BINARY_DIR}/graphics-engine/examples/bin)
endif(WIN32)



#IHMProject
project(seed_IHM)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

if(WIN32)
	set(CMAKE_PREFIX_PATH ${LIBS_DIR}\Windows\Qt\5.5\msvc2013_64)
else(WIN32)
	set(CMAKE_PREFIX_PATH ~/Qt/5.7/gcc_64)
endif(WIN32)

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Seed/IHM)
# GLM include directories
include_directories(${GLM_INCLUDE_DIR})
# include Materials
include_directories(${CMAKE_SOURCE_DIR}/include/Seed/Graphics/Materials)
# AntTweakBar include directories
include_directories(${ANTTWEAKBAR_INCLUDE_DIR})
# Boost include directories
#include_directories(${BOOST_INCLUDE_DIR})
# Assimp include directories
include_directories(${ASSIMP_INCLUDE_DIR})
# GLFW include directories
include_directories(${GLFW_INCLUDE_DIR})

SET(Qt_SOURCES ${CMAKE_SOURCE_DIR}/src/Seed/IHM/main.cpp ${CMAKE_SOURCE_DIR}/src/Seed/IHM/myglwidget.cpp ${CMAKE_SOURCE_DIR}/src/Seed/IHM/glwidget.cpp ${CMAKE_SOURCE_DIR}/src/Seed/IHM/mainwindow.cpp  ${CMAKE_SOURCE_DIR}/src/Seed/IHM/myglwidgetcontrol.cpp)
SET(Qt_HEADERS_MOC ${CMAKE_SOURCE_DIR}/include/Seed/IHM/myglwidget.hpp ${CMAKE_SOURCE_DIR}/include/Seed/IHM/glwidget.hpp ${CMAKE_SOURCE_DIR}/include/Seed/IHM/mainwindow.hpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/IHM/bin)

QT5_WRAP_CPP(manager_moc ${Qt_HEADERS_MOC})

add_executable(Seed-IHM ${Qt_SOURCES} ${Qt_HEADERS_MOC})

if(WIN32)
	target_link_libraries(Seed-IHM opengl32.lib)
	target_link_libraries(Seed-IHM ${CMAKE_SOURCE_DIR}/lib-required/glfw_win32/lib-vc2013/glfw3.lib)
	target_link_libraries(Seed-IHM ${CMAKE_SOURCE_DIR}/lib-required/antTweakBar/lib/AntTweakBar.lib)
	target_link_libraries(Seed-IHM ${CMAKE_SOURCE_DIR}/lib-required/assimp_win32/lib/assimp_release-dll_win32/assimp.lib)
	target_link_libraries(Seed-IHM ${CMAKE_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_system-vc120-mt-1_59.lib)
	target_link_libraries(Seed-IHM ${CMAKE_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_filesystem-vc120-mt-1_59.lib)
	target_link_libraries(Seed-IHM ${PROJECT_BINARY_DIR}/graphics-engine/Release/Seed-Graphics.lib)
else(WIN32)
	target_link_libraries(Seed-IHM ${ASSIMP_LIB_LINUX})
	target_link_libraries(Seed-IHM ${GLFW_LIB_LINUX})
	target_link_libraries(Seed-IHM ${ANTTWEAKBAR_LIB_LINUX})
	#target_link_libraries(Seed-IHM ${BOOST_FILESYSTEM_LIB_LINUX})
	#target_link_libraries(Seed-IHM ${BOOST_SYSTEM_LINUX})
	target_link_libraries(Seed-IHM ${PROJECT_BINARY_DIR}/graphics-engine/lib/libSeed-Graphics.so)
	target_link_libraries(Seed-IHM ${OPENGL_INCLUDE_DIRS})
	target_link_libraries(Seed-IHM ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
	if(GLEW_FOUND)
		target_link_libraries(Seed-IHM ${GLEW_LIBRARIES})
	else(GLEW_FOUND)
		target_link_libraries(Seed-IHM ${GLEW_LIB_LINUX})
	endif(GLEW_FOUND)
	if(Boost_FOUND)
		target_link_libraries(Seed-IHM ${Boost_LIBRARIES})
	else(Boost_FOUND)
		target_link_libraries(Seed-IHM ${BOOST_SYSTEM_LINUX})
		target_link_libraries(Seed-IHM ${BOOST_FILESYSTEM_LIB_LINUX})
	endif(Boost_FOUND)
endif(WIN32)


qt5_use_modules(Seed-IHM Widgets Core OpenGL Gui)

if(WIN32)
	#COPY FILES INTO THE BINARY PROJECT FILES
	file(GLOB DLL ${CMAKE_SOURCE_DIR}/lib-required/DLL/*.dll)
	file(COPY ${DLL} DESTINATION ${PROJECT_BINARY_DIR}/IHM/bin/Release)
	file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${PROJECT_BINARY_DIR}/IHM/bin/Release)
	file(GLOB ICONES ${CMAKE_SOURCE_DIR}/ressources/IHM/*)
	file(COPY ${ICONES} DESTINATION ${PROJECT_BINARY_DIR}/IHM/bin/Release)
else(WIN32)
	file(GLOB ICONES ${CMAKE_SOURCE_DIR}/ressources/IHM/*)
	file(COPY ${ICONES} DESTINATION ${PROJECT_BINARY_DIR}/IHM/bin)
	file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${PROJECT_BINARY_DIR}/IHM/bin)
endif(WIN32)
