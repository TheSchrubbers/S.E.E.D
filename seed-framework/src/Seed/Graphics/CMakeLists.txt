project(seed_GRAPHICS CXX)

file(
    GLOB_RECURSE
    GRAPHICS_SRC
    ${CMAKE_SOURCE_DIR}/src/Seed/Graphics/*.cpp
    ${CMAKE_SOURCE_DIR}/include/Seed/Graphics/*.cpp
    ${CMAKE_SOURCE_DIR}/src/Seed/Graphics/*.hpp
    ${CMAKE_SOURCE_DIR}/include/Seed/Graphics/*.hpp
    ${CMAKE_SOURCE_DIR}/ressources/*.hpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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
	target_link_libraries(Seed-Graphics ${CMAKE_CURRENT_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_filesystem-vc120-mt-1_59.lib)
else(WIN32)
	target_link_libraries(Seed-Graphics ${ASSIMP_LIB_LINUX})
    #target_link_libraries(Seed-Graphics ${Boost_FILESYSTEM_LIBRARIES} ${Boost_SYSTEM_LIBRARIES} ${Boost_REGEX_LIBRARY})
    target_link_libraries(Seed-Graphics ${BOOST_SYSTEM_LIB_LINUX} ${BOOST_FILESYSTEM_LIB_LINUX})
	target_link_libraries(Seed-Graphics ${QT_LIBRARIES})
endif(WIN32)

qt5_use_modules(Seed-Graphics Widgets Core OpenGL Gui)

SET_TARGET_PROPERTIES(Seed-Graphics PROPERTIES OUTPUT_NAME Seed-Graphics)
