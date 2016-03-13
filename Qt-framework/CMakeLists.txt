cmake_minimum_required(VERSION 2.8.11)
#QtProject
get_filename_component(ProjectId ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if(WIN32)
else(WIN32)
	set(CMAKE_PREFIX_PATH /home/chtimy/Qt/5.5/gcc_64/lib/cmake/Qt5Core)
endif(WIN32)

FIND_PACKAGE(Qt5Core REQUIRED)
get_target_property(QtCore_location Qt5::Core LOCATION)

if(WIN32)
else(WIN32)
	set(CMAKE_PREFIX_PATH /home/chtimy/Qt/5.5/gcc_64/lib/cmake/Qt5Widgets)
endif(WIN32)

FIND_PACKAGE(Qt5Widgets REQUIRED)
get_target_property(QtCore_location Qt5::Widgets LOCATION)

foreach(plugin ${Qt5Core_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  message("Plugin ${plugin} is at location ${_loc}")
endforeach()
foreach(plugin ${Qt5Widgets_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  message("Plugin ${plugin} is at location ${_loc}")
endforeach()

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

SET(Qt_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp ${CMAKE_SOURCE_DIR}/src/window.cpp ${CMAKE_SOURCE_DIR}/src/glwidget.cpp)
SET(Qt_HEADERS ${CMAKE_SOURCE_DIR}/include/main.hpp)
SET(Qt_HEADERS_MOC ${CMAKE_SOURCE_DIR}/include/window.hpp ${CMAKE_SOURCE_DIR}/include/glwidget.hpp)

#QT5_WRAP_CPP(Qt_HEADERS_MOC ${Qt_HEADERS})
#INCLUDE(${QT_USE_FILE})
#ADD_DEFINITIONS(${QT_DEFINITIONS})

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

QT5_WRAP_CPP(manager_moc ${Qt_HEADERS_MOC})

add_executable(${ProjectId} ${Qt_SOURCES} ${Qt_HEADERS_MOC})

target_link_libraries(${ProjectId} Qt5::Core)
target_link_libraries(${ProjectId} Qt5::Widgets)

if(WIN32)
	target_link_libraries(${ProjectId} opengl32.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/../seed-framework/lib-required/glfw_win32/lib-vc2013/glfw3.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/../seed-framework/lib-required/glew_win32/lib/Release/Win32/glew32.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/../seed-framework/lib-required/antTweakBar/lib/AntTweakBar.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/../seed-framework/lib-required/assimp_win32/lib/assimp_release-dll_win32/assimp.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/../seed-framework/lib-required/boost_win32/lib/libboost_system-vc120-mt-1_59.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/../seed-framework/lib-required/boost_win32/lib/libboost_filesystem-vc120-mt-1_59.lib)
	target_link_libraries(${ProjectId} ${CMAKE_SOURCE_DIR}/../seed-framework/build/Release/Seed-Graphics.lib)
else(WIN32)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/../seed-framework/lib-required/Linux/assimp/lib/libassimp.so.3.2.0)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/../seed-framework/lib-required/Linux/glfw/lib/libglfw.so.3.1)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/../seed-framework/lib-required/Linux/boost/lib/libboost_system.so.1.59.0)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/../seed-framework/lib-required/Linux/boost/lib/libboost_filesystem.so.1.59.0)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/../seed-framework/lib-required/Linux/AntTweakBar/lib/libAntTweakBar.so.1.0)
	#target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/Linux/glew/lib/libGLEW.so.1.13.0)
	target_link_libraries(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/../seed-framework/build/lib/libSeed-Graphics.so)
	target_link_libraries(${ProjectId} ${OPENGL_INCLUDE_DIRS})
	if(GLEW_FOUND)
		target_link_libraries(${ProjectId} ${GLEW_LIBRARIES})
	endif(GLEW_FOUND)
endif(WIN32)

if(WIN32)
	#COPY FILES INTO THE BINARY PROJECT FILES
	#file(GLOB DLL
	#	${CMAKE_SOURCE_DIR}/lib-required/DLL/*.dll
	#)
	#file(COPY ${DLL} DESTINATION ${PROJECT_BINARY_DIR}/bin/Release)
	#file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${PROJECT_BINARY_DIR}/bin/Release)
else(WIN32)
	#file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${PROJECT_BINARY_DIR}/bin)
endif(WIN32)
