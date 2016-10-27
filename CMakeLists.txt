# cmake requirements
cmake_minimum_required(VERSION 2.8)

# Build options have to be before PROJECT(...)
SET(CMAKE_CONFIGURATION_TYPES "Release" CACHE PATH "Configuration types")
SET(CMAKE_BUILD_TYPE "Release" CACHE PATH "Current build configuration")

# Example Project configuration
PROJECT(PUTAR)
if(APPLE)
    SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -std=c++11 -pthread")
else()
    SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -std=c++11 -pthread -malign-double")
endif()

#add_definitions(-DDEBUG)

# Folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Settings
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/resources")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/putar")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/putar/Defs")

# core libraries
option(BUILD_PUTAR "Build PUTAR libraries" YES)
option(BUILD_DEFS "Build PUTAR Defs library" YES)
mark_as_advanced(BUILD_DEFS)
option(BUILD_PUTAR_UTILITIES "Build PUTAR Utilities library" ON)
mark_as_advanced(BUILD_PUTAR_UTILITIES)
option(BUILD_VISUALIZER "Build Visualizer" ON)
mark_as_advanced(BUILD_VISUALIZER)
option(BUILD_OBJLOADER "Build ObjLoader" ON)
mark_as_advanced(BUILD_OBJLOADER)
option(BUILD_VIS3D "Build Vis3D" ON)
mark_as_advanced(BUILD_VIS3D)

# demos
option(BUILD_PUTAR_DEMO "Build PUTAR demonstration programs and libraries" ON)
option(BUILD_PUTAR_DEMO_MAIN "Build main PUTAR test" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_MAIN)
option(BUILD_PUTAR_DEMO_VISUALIZER "Build PUTAR visualizer demo" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_VISUALIZER)

#additional dependencies
# Boost
FIND_PACKAGE(Boost COMPONENTS system filesystem thread asio)

# OpenCV
FIND_PACKAGE(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

#Eigen
if(NOT APPLE)
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/3rdParty/Eigen)
endif()
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(-Wno-deprecated -DEIGEN_DONT_VECTORIZE -DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT)

if (BUILD_PUTAR)
    #QGLViewer library
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
    FIND_PACKAGE(OpenGL)
    include_directories(${OPENGL_INCLUDE_DIR})
    link_directories(${OPENGL_LIBRARY})

    #QGLViewer library
    FIND_PACKAGE(QGLViewer)
    include_directories(${QGLVIEWER_INCLUDE_DIR})
    include_directories(${QT_INCLUDES})
    link_directories(${QGLVIEWER_LIBRARY})

    # Architecture
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            SET(X86_64 1)
            #MESSAGE("Architecture x86 64 bit")
    else(CMAKE_SIZEOF_VOID_P EQUAL 8)
            SET(X86_64 0)
            #MESSAGE("Architecture x86 32 bit")
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

endif(BUILD_PUTAR)

# Executable output directory
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Executable output directory")
mark_as_advanced(RUNTIME_OUTPUT_DIRECTORY)

# Dynamic library output directory
SET(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Dynamic library output directory")
mark_as_advanced(LIBRARY_OUTPUT_DIRECTORY)

# Static library output directory
SET(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Static library output directory")
mark_as_advanced(ARCHIVE_OUTPUT_DIRECTORY)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_OUTPUT_DIRECTORY})
foreach(CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${CONFIGURATION_TYPE} CONFIGURATION_TYPE)
        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${RUNTIME_OUTPUT_DIRECTORY})
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${LIBRARY_OUTPUT_DIRECTORY})
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${ARCHIVE_OUTPUT_DIRECTORY})
endforeach(CONFIGURATION_TYPE CMAKE_CONFIGURATION_TYPES)

if(MSVC)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

###############################################################################
#
# PUTAR tinyXML2 library
#
###############################################################################

#add_library(tinyxml2 SHARED ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.h ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.cpp)
add_library(tinyxml2 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.h ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.cpp)

###############################################################################
#
# PUTAR defs library
#
###############################################################################

if(BUILD_DEFS)
        if(APPLE)
          file(GLOB DEFS_SOURCES
              "${CMAKE_SOURCE_DIR}/src/Defs/*.cpp"
          )
        endif()
        file(GLOB DEFS_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/Defs/*.h"
        )
        if(APPLE)
            add_library(PUTARDefs STATIC ${DEFS_SOURCES} ${DEFS_HEADERS})
        else()
            add_library(PUTARDefs STATIC ${DEFS_HEADERS})
            set_target_properties(PUTARDefs PROPERTIES LINKER_LANGUAGE CXX)
        endif()
# 	INSTALL(TARGETS PUTARDefs RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
        INSTALL(FILES ${DEFS_HEADERS} DESTINATION include/putar/Defs/)

endif(BUILD_DEFS)

###############################################################################
#
# PUTAR Utilities library
#
###############################################################################

if(BUILD_PUTAR_UTILITIES)
        file(GLOB UTILITIES_SOURCES
            "${CMAKE_SOURCE_DIR}/src/Utilities/*.cpp"
        )
        file(GLOB UTILITIES_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/Utilities/*.h"
        )
        add_library(Utilities STATIC ${UTILITIES_SOURCES} ${UTILITIES_HEADERS})
#         INSTALL(TARGETS Utilities RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
        INSTALL(FILES ${TRACKER_HEADERS} DESTINATION include/putar/Utilities/)
endif(BUILD_PUTAR_UTILITIES)

###############################################################################
#
# PUTAR Visualizer library
#
###############################################################################

if(BUILD_PUTAR AND BUILD_VISUALIZER)
        file(GLOB VISUALIZER_SOURCES
            "${CMAKE_SOURCE_DIR}/src/Visualizer/*.cpp"
        )
        file(GLOB VISUALIZER_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/Visualizer/*.h"
        )
        ADD_LIBRARY(Visualizer STATIC ${VISUALIZER_SOURCES} ${VISUALIZER_HEADERS})
        if(APPLE)
            TARGET_LINK_LIBRARIES(Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY})
        else()
            TARGET_LINK_LIBRARIES(Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY})
        endif()

#        INSTALL(TARGETS Utilities RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
#        INSTALL(FILES ${VISUALIZER_HEADERS} DESTINATION include/walkers/Visualizer/)
endif(BUILD_PUTAR AND BUILD_VISUALIZER)

###############################################################################
#
# PUTAR ObjLoader library
#
###############################################################################

if(BUILD_PUTAR AND BUILD_OBJLOADER)
        file(GLOB VISUALIZER_SOURCES
            "${CMAKE_SOURCE_DIR}/src/ObjLoader/*.cpp"
        )
        file(GLOB VISUALIZER_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/ObjLoader/*.h"
        )
        ADD_LIBRARY(ObjLoader STATIC ${VISUALIZER_SOURCES} ${VISUALIZER_HEADERS})
        TARGET_LINK_LIBRARIES(ObjLoader tinyxml2)
#        INSTALL(TARGETS Utilities RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
#        INSTALL(FILES ${VISUALIZER_HEADERS} DESTINATION include/walkers/Visualizer/)
endif(BUILD_PUTAR AND BUILD_OBJLOADER)

###############################################################################
#
# Vis3D library
#
###############################################################################

if(BUILD_PUTAR AND BUILD_VIS3D)
        file(GLOB VIS3D_SOURCES
            "${CMAKE_SOURCE_DIR}/src/Vis3D/*.cpp"
        )
        file(GLOB VIS3D_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/Vis3D/*.h"
        )
        ADD_LIBRARY(Vis3D STATIC ${VIS3D_SOURCES} ${VIS3D_HEADERS})
        TARGET_LINK_LIBRARIES(Vis3D tinyxml2)
endif(BUILD_PUTAR AND BUILD_VIS3D)

###############################################################################
#
# PUTAR demo Visualizer
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO_VISUALIZER)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./demos/demoVisualizer.cpp)
        ADD_EXECUTABLE(demoVisualizer ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(demoVisualizer Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
        else()
            TARGET_LINK_LIBRARIES(demoVisualizer Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} glut)
        endif()
        INSTALL(TARGETS demoVisualizer RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO_VISUALIZER)

###############################################################################
#
# PUTAR demo
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO AND BUILD_PUTAR_DEMO_MAIN)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./main.cpp)
        ADD_EXECUTABLE(putar ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(putar PUTARDefs Utilities Vis3D ObjLoader tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
        else()
            TARGET_LINK_LIBRARIES(putar PUTARDefs Utilities Vis3D ObjLoader tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} glut)
        endif()
        INSTALL(TARGETS putar RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO AND BUILD_PUTAR_DEMO_MAIN)
