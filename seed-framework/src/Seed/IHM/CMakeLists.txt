#IHMProject
project(seed_IHM)

SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Seed/IHM)

SET(Qt_SOURCES ${CMAKE_SOURCE_DIR}/src/Seed/IHM/main.cpp ${CMAKE_SOURCE_DIR}/src/Seed/IHM/myglwidget.cpp ${CMAKE_SOURCE_DIR}/src/Seed/IHM/glwidget.cpp ${CMAKE_SOURCE_DIR}/src/Seed/IHM/mainwindow.cpp  ${CMAKE_SOURCE_DIR}/src/Seed/IHM/myglwidgetcontrol.cpp)
SET(Qt_HEADERS_MOC ${CMAKE_SOURCE_DIR}/include/Seed/IHM/myglwidget.hpp ${CMAKE_SOURCE_DIR}/include/Seed/IHM/glwidget.hpp ${CMAKE_SOURCE_DIR}/include/Seed/IHM/mainwindow.hpp)

QT5_WRAP_CPP(manager_moc ${Qt_HEADERS_MOC})

add_executable(Seed-IHM ${Qt_SOURCES} ${Qt_HEADERS_MOC})

if(WIN32)
	target_link_libraries(Seed-IHM ${CMAKE_SOURCE_DIR}/lib-required/assimp_win32/lib/assimp_release-dll_win32/assimp.lib)
	target_link_libraries(Seed-IHM ${CMAKE_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_system-vc120-mt-1_59.lib)
	target_link_libraries(Seed-IHM ${CMAKE_SOURCE_DIR}/lib-required/boost_win32/lib/libboost_filesystem-vc120-mt-1_59.lib)
	target_link_libraries(Seed-IHM ${CMAKE_BINARY_DIR}/lib/Seed-Graphics.lib)
else(WIN32)
	target_link_libraries(Seed-IHM ${ASSIMP_LIB_LINUX})
	#target_link_libraries(Seed-IHM ${Boost_LIBRARIES})
	target_link_libraries(Seed-IHM ${QT_LIBRARIES})
	target_link_libraries(Seed-IHM ${CMAKE_BINARY_DIR}/lib/libSeed-Graphics.so)
endif(WIN32)

qt5_use_modules(Seed-IHM Widgets Core OpenGL Gui)

ADD_DEPENDENCIES( Seed-IHM Seed-Graphics)
