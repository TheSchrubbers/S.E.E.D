cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

SET(CMAKE_CONFIGURATION_TYPES Release Debug CACHE STRING "" FORCE)

if(WIN32)
	set(CMAKE_PREFIX_PATH ${LIBS_DIR}\Windows\Qt\5.5\msvc2013_64)
else(WIN32)
	#set(CMAKE_PREFIX_PATH ~/Qt/5.7/gcc_64)
	set(CMAKE_PREFIX_PATH /opt/Qt/5.7/gcc_64)
endif(WIN32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -isystemglm")

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)

#set(CMAKE_PREFIX_PATH lib-required/lib/Linux/Boost/)
#FIND_PACKAGE(Boost COMPONENTS regex system filesystem REQUIRED)

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

ENABLE_LANGUAGE(C)

set(INCLUDES_DIR ${CMAKE_SOURCE_DIR}/lib-required/include)
set(ASSIMP_INCLUDE_DIR ${INCLUDES_DIR})
set(ANTTWEAKBAR_INCLUDE_DIR ${INCLUDES_DIR}/antTweakBar)
set(BOOST_INCLUDE_DIR ${INCLUDES_DIR})
set(GLFW_INCLUDE_DIR ${INCLUDES_DIR}/glfw)
set(GLM_INCLUDE_DIR ${INCLUDES_DIR}/glm)
set(GLEW_INCLUDE_DIR ${INCLUDES_DIR}/glew)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/lib-required/lib)
set(ASSIMP_LIB_LINUX ${LIBS_DIR}/Linux/assimp/libassimp.so.3)
set(GLFW_LIB_LINUX ${LIBS_DIR}/Linux/glfw/libglfw.so.3.1)
set(BOOST_SYSTEM_LIB_LINUX ${LIBS_DIR}/Linux/Boost/libboost_system.so)
set(BOOST_FILESYSTEM_LIB_LINUX ${LIBS_DIR}/Linux/Boost/libboost_filesystem.so)
set(Boost_USE_MULTITHREADED ON)
set(GLEW_LIB_LINUX ${LIBS_DIR}/Linux/glew/libGLEW.so.1.13.0)
set(ANTTWEAKBAR_LIB_LINUX ${LIBS_DIR}/Linux/AntTweakBar/libAntTweakBar.a)

# GLM include directories
include_directories(${GLM_INCLUDE_DIR})
# include Materials
include_directories(${CMAKE_SOURCE_DIR}/include/Seed/Graphics/Materials)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)
# Boost include directories
#include_directories(${BOOST_INCLUDE_DIR})
# Assimp include directories
include_directories(${ASSIMP_INCLUDE_DIR})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Seed/Graphics)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Seed/IHM)

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
