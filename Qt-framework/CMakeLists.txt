#QtProject
get_filename_component(ProjectId ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

FIND_PACKAGE(Qt5 REQUIRED)

SET(Qt_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
SET(Qt_HEADERS ${CMAKE_SOURCE_DIR}/include/*.hpp)

QT5_WRAP_CPP(Qt_HEADERS_MOC ${Qt_HEADERS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

file(
    GLOB_RECURSE
    QT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../QtProject/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../QtProject/include/*.cpp
)

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